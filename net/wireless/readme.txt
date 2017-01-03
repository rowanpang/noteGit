0,useage
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

3,with selinux
    默认情况，不允许
        type=AVC msg=audit(1459150950.827:90): avc:  denied  { write } for  pid=7446 comm="wpa_supplicant" name="wpa_ctrl_7448-1" 
            dev="sda9" ino=2490636 scontext=unconfined_u:system_r:NetworkManager_t:s0 tcontext=unconfined_u:object_r:tmp_t:s0 tclass=sock_file permissive=0
    通过，audit2allow 生成规则.
        ref selinux/policy/allowAudit.txt

