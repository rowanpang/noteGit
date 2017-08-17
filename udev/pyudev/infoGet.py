import os
import pyudev

ctx = pyudev.Context()
print ctx.sys_path

dev = pyudev.Devices.from_path(ctx, '/sys/block/sda')
dev = pyudev.device.Devices.from_name(ctx,'block','sda')
dev = pyudev.device.Devices.from_device_file(ctx,'/dev/sda')
print type(dev)
print dev.properties
print dev['ID_TYPE']
for k in dev.properties:
    print '%s = %s' %(k,dev.properties[k])


for dev in ctx.list_devices(subsystem='block',DEVTYPE='disk'):
    print dev.get('ID_TYPE')

