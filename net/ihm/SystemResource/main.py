#!/usr/bin/python
#coding:utf-8
from flask import Flask, jsonify
from flask import Blueprint, abort
import os
import threading
import time
import re

INTERVAL = 3

SystemResourceRoute = Blueprint('SystemResource', __name__)

global_network_history = []
global_cpu_history = []
global_last_cpu_stat = None
global_cpu_num = 0
global_mem_history = []
re_RX_packets = re.compile(r'RX packets:(\d*)')
re_TX_packets = re.compile(r'TX packets:(\d*)')
re_RX_bytes = re.compile(r'RX bytes:(\d*)')
re_TX_bytes = re.compile(r'TX bytes:(\d*)')
class CollectThreading(threading.Thread):
	def __init__(self, threadname):
                threading.Thread.__init__(self, name = threadname)

	def collect_CPU(self):
		global global_cpu_history, global_last_cpu_stat, global_cpu_num
		if global_cpu_num == 0:
			global_cpu_num = open('/proc/cpuinfo').read().count('processor')
		this_cpu_stat = []
		for line in open('/proc/stat').readlines()[1:global_cpu_num+1]:
			idle_cpu_time = int(line.split()[4])
			total_cpu_time = sum(map(int, line.split()[1:8]))
			this_cpu_stat.append((idle_cpu_time, total_cpu_time))
		if global_last_cpu_stat == None:
			global_last_cpu_stat = this_cpu_stat
			return
		l = []
		for i in range(global_cpu_num):
			this_idle_cpu_time, this_total_cpu_time = this_cpu_stat[i]
			last_idle_cpu_time, last_total_cpu_time = global_last_cpu_stat[i]
			cpu_usage = 1.0 - 1.0*(this_idle_cpu_time - last_idle_cpu_time)/(this_total_cpu_time - last_total_cpu_time)
			l.append("%.2f" % cpu_usage)
		global_cpu_history.append({'time':int(time.time()), 'cpu':l})
		global_last_cpu_stat = this_cpu_stat

	def collect(self):
		global global_network_history, global_mem_history

		# Network IO
		output = os.popen('ifconfig wlan0').read()
		rx_bytes_total = int(re_RX_bytes.search(output).group(1))
		tx_bytes_total = int(re_TX_bytes.search(output).group(1))
		rx_packets_total = int(re_RX_packets.search(output).group(1))
		tx_packets_total = int(re_TX_packets.search(output).group(1))
		d = {'time':int(time.time()), 'rx_packets_total':rx_packets_total, 'tx_packets_total':tx_packets_total, 'rx_bytes_total':rx_bytes_total, 'tx_bytes_total':tx_bytes_total}
		global_network_history.append(d)
		
		# CPU usage
		self.collect_CPU()

		# Memory/Swap usage
		x, mem_line, xx, swap_line = os.popen('free').readlines()
		mem_total, mem_usage = map(int, mem_line.split()[1:3])
		swap_total, swap_usage = map(int, swap_line.split()[1:3])
		d = {'time':int(time.time()), 'mem_total':mem_total, 'mem_usage':mem_usage, 'swap_total':swap_total, 'swap_usage':swap_usage}
		global_mem_history.append(d)

        def run(self):
		while True:
			self.collect()
			time.sleep(INTERVAL)


blacklist_mountpoints = ['/dev', '/run', '/dev/shm', '/run/lock', '/sys/fs/cgroup', '/run/user/0']
blacklist_filesystems = ['tmpfs', 'udev']

@SystemResourceRoute.route('/PartitionMounts')
def show_mounts():
	ret_dict = {}
	for line in os.popen('df -h').readlines():
		line = line.strip()
		filesystem, total, used, available, used_percent, mount_point = line.split()
		if filesystem in blacklist_filesystems:
			continue
		if mount_point in blacklist_mountpoints:
			continue
		if not mount_point.startswith('/'):
			continue
		ret_dict[mount_point] = {'total':total, 'used':used, 'available':available, 'used_percent':used_percent, 'filesystem':filesystem}
	for line in os.popen('mount').readlines():
		line = line.strip()
		filesystem, x, mount_point, xx, type, mount_ext = line.split(' ', 5)
		if ret_dict.has_key(mount_point):
			ret_dict[mount_point]['type'] = type
	return jsonify(ret_dict)

@SystemResourceRoute.route('/CurrentNetworkIORate')
def show_current_network_io_rate():
	global global_network_history
	ret_dict = {}
	ret_dict['rx_bytes_rate'] = 0
	ret_dict['tx_bytes_rate'] = 0
	ret_dict['rx_packets_rate'] = 0
	ret_dict['tx_packets_rate'] = 0
	if len(global_network_history) < 2:
		return jsonify(ret_dict)
	history1, history2 = global_network_history[-2:]
	rx_bytes_rate = (history2['rx_bytes_total'] - history1['rx_bytes_total'])/INTERVAL
	tx_bytes_rate = (history2['tx_bytes_total'] - history1['tx_bytes_total'])/INTERVAL
	rx_packets_rate = (history2['rx_packets_total'] - history1['rx_packets_total'])/INTERVAL
	tx_packets_rate = (history2['tx_packets_total'] - history1['tx_packets_total'])/INTERVAL
	ret_dict['rx_bytes_rate'] = rx_bytes_rate
	ret_dict['tx_bytes_rate'] = tx_bytes_rate
	ret_dict['rx_packets_rate'] = rx_packets_rate
	ret_dict['tx_packets_rate'] = tx_packets_rate
	return jsonify(ret_dict)

re_Disk_name_size = re.compile(r'Disk ([^:]*): ([^,]*),')
re_Disk_Model = re.compile(r'Model=(.*),')

@SystemResourceRoute.route('/SystemInformation')
def show_system_information():
	ret_dict = {}
	# CPU
	for line in open('/proc/cpuinfo').readlines():
		if line.startswith('model name'):
			ret_dict['CPUModel'] = line.split(':')[1].strip()
		if line.startswith('cpu MHz'):
			ret_dict['CPUFreq'] = line.split(':')[1].strip()

	# DISK
	output = os.popen('fdisk -l').read()
	name, size = re_Disk_name_size.search(output).group(1, 2)
	ret_dict['DiskSize'] = size
	output = os.popen('hdparm -i %s' % name).read()
	ret_dict['DiskModel'] = re_Disk_Model.search(output).group(1)

	# Audio, Video, NetCard
	for line in os.popen('lspci').readlines():
		if line.find('Audio') != -1:
			ret_dict['AudioCard'] = line.split(':')[2].split('(')[0].strip()
		if line.find('VGA') != -1:
			ret_dict['VideoCard'] = line.split(':')[2].split('(')[0].strip()
		if line.find('Ethernet') != -1:
			ret_dict['NetCard'] = line.split(':')[2].split('(')[0].strip()

	# MainBoard
	ret_dict['Mainboard'] = os.popen('dmidecode | grep -A 1 "System Information"').readlines()[1].split(':')[1].strip()

	return jsonify(ret_dict)

@SystemResourceRoute.route('/CurrentCpuUsage')
def show_current_cpu_usage():
	global global_cpu_history
	history = global_cpu_history[-1]
	return jsonify(history['cpu'])

@SystemResourceRoute.route('/CurrentMemUsage')
def show_current_mem_usage():
	global global_mem_history
	history = global_mem_history[-1]
	mem_usage = history['mem_usage']
	mem_total = history['mem_total']
	ret_dict = {'mem_usage_percent': "%.2f" % (1.0*mem_usage/mem_total)}
	return jsonify(ret_dict)

@SystemResourceRoute.route('/CurrentSwapUsage')
def show_current_swap_usage():
	global global_mem_history
	history = global_mem_history[-1]
	swap_usage = history['swap_usage']
	swap_total = history['swap_total']
	ret_dict = {'swap_usage_percent': "%.2f" % (1.0*swap_usage/swap_total)}
	return jsonify(ret_dict)

if __name__ == '__main__':
	app = Flask(__name__)
	app.register_blueprint(SystemResourceRoute, url_prefix='')
	collect_thread = CollectThreading('collect thread')
	collect_thread.setDaemon(True)
	collect_thread.start()
	app.run(host = '0.0.0.0', port=8080)
