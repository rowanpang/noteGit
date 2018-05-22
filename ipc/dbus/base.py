#!/usr/bin/python2.7
import dbus
bus = dbus.SystemBus()
ud_manager_obj = bus.get_object('org.freedesktop.UDisks2', '/org/freedesktop/UDisks2')
om = dbus.Interface(ud_manager_obj, 'org.freedesktop.DBus.ObjectManager')
devices=[]
device={}
for v in om.GetManagedObjects().itervalues():
    blockInfo = v.get('org.freedesktop.UDisks2.Block', {})
    if blockInfo.get('IdUsage')=="filesystem" and not blockInfo.get('ReadOnly'): 
        device['IdLabel'] = blockInfo.get('IdLabel')
        device['IdUUID'] = blockInfo.get('IdUUID')
        tmp= blockInfo.get('Device')
        device['devNode'] = bytearray(tmp).replace(b'\x00', b'').decode('utf-8')
        driver = blockInfo.get('Drive')
        driverObj = bus.get_object('org.freedesktop.UDisks2',driver)
        driverIf = dbus.Interface(driverObj,'org.freedesktop.DBus.Properties')
        device['Vendor'] = driverIf.Get('org.freedesktop.UDisks2.Drive','Vendor')
        device['Model'] = driverIf.Get('org.freedesktop.UDisks2.Drive','Model')
        device['Removable'] = driverIf.Get('org.freedesktop.UDisks2.Drive','Removable')
        device['ConnectionBus'] = driverIf.Get('org.freedesktop.UDisks2.Drive','ConnectionBus')
        if device['ConnectionBus'] == 'usb':
            devices.append(device)
            print device
            print devices

    # if blockInfo.get('ConnectionBus') == 'usb' and drive_info.get('Removable'):
        # if drive_info['MediaRemovable']:
            # print("Device Path: %s" % k)
