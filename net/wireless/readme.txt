use:
	a,复制wpa_supplicant 到/etc/init.d路径
		wpa_supplicant stop
		wpa_supplicant start
		...
	b,复制 99-wlanudev.rules 到 /etc/udev/rules.d
		实现无线网卡设备的插拔的自动认证

1,wlandhcp.sh
	主要实现wpa_supplicant认证成功及中间断开时的dhcp相关处理.

2,wpa_supplicant
	对系统自带的/etc/init.d/wpa_supplicant加以修改，
		start:增加了wpa_cli action file
		stop: 调用wlandhcp.sh,release dhclient

