#!/usr/bin/python

import sys
import os
sys.path.insert(0,os.path.dirname(__file__) + '/site-packages')

import ctypes

ctypes.cdll.LoadLibrary('./site-packages/libpcap/libpcap.so')


import pcap

for l in open('/proc/self/maps'):
    print l,
