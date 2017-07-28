#!/usr/bin/env python
# Parse the initial bytes from a FAT-fs image.
# 2012-04-08
import struct

# formats of various parts of a FAT boot sector:

#----
#offsets into boot_sector fields:
FAT_TYPE_12=0
FAT_TYPE_16=1
FAT_TYPE_32=2

KEY_BPB_BASE_JmpBoot        = 'JmpBoot'
KEY_BPB_BASE_OEMName        = 'OEMName'
KEY_BPB_BASE_BytsPerSec	    = 'BytsPerSec'
KEY_BPB_BASE_SecPerClus	    = 'SecPerClus'
KEY_BPB_BASE_RsvdSecs	    = 'RsvdSecs'
KEY_BPB_BASE_NumFATs	    = 'NumFATs'
KEY_BPB_BASE_RootEntCnt	    = 'RootEntCnt'
KEY_BPB_BASE_TotSec16	    = 'TotSec16'
KEY_BPB_BASE_MediaDesp	    = 'MediaDesp'
KEY_BPB_BASE_SecPerFAT16    = 'SecPerFAT16'
KEY_BPB_BASE_SecPerTrk	    = 'SecPerTrk'
KEY_BPB_BASE_NumHeads	    = 'NumHeads'
KEY_BPB_BASE_HidenSec	    = 'HidenSec'
KEY_BPB_BASE_TotSec32	    = 'TotSec32'

KEY_BPB_EXT16_PhyDrvNum	    = 'PhyDrvNum'
KEY_BPB_EXT16_Rsvd1	    = 'Rsvd1'
KEY_BPB_EXT16_BootSig	    = 'BootSig'
KEY_BPB_EXT16_VolID	    = 'VolID'
KEY_BPB_EXT16_VolLabel	    = 'VolLabel'
KEY_BPB_EXT16_FilSysType    = 'FilSysType'

KEY_BPB_EXT32_SecPerFAT32   = 'SecPerFAT32'
KEY_BPB_EXT32_DrvDesp	    = 'DrvDesp'
KEY_BPB_EXT32_FSVer	    = 'FSVer'
KEY_BPB_EXT32_RootClus	    = 'RootClus'
KEY_BPB_EXT32_FSInfo	    = 'FSInfo'
KEY_BPB_EXT32_BkBootSec	    = 'BkBootSec'
KEY_BPB_EXT32_Rsvd2	    = 'Rsvd2'
KEY_BPB_EXT32_PhyDrvNum	    = 'PhyDrvNum'
KEY_BPB_EXT32_Rsvd1	    = 'Rsvd1'
KEY_BPB_EXT32_BootSig	    = 'BootSig'
KEY_BPB_EXT32_VolID	    = 'VolID'
KEY_BPB_EXT32_VolLabel	    = 'VolLabel'
KEY_BPB_EXT32_FilSysType    = 'FilSysType'

media_discription_byte = { \
    0xf0 : ('1.44 MB / 2.88 MB', \
            '3.5-inch, 2-sided, 18-sector / 3.5-inch, 2-sided, 36-sector') ,
    0xf9 : ( '720 KB / 1.2 MB', \
            '3.5-inch, 2-sided, 9-sector / 5.25-inch, 2-sided, 15-sector') ,
    0xfd : ( '360 KB', '5.25-inch, 2-sided, 9-sector') ,
    0xff : ( '320 KB', '5.25-inch, 2-sided, 8-sector') ,
    0xfc : ( '180 KB', '5.25-inch, 1-sided, 9-sector') ,
    0xfe : ( '160 KB', '5.25-inch, 1-sided, 8-sector') ,
    0xf8 : (  '-----', 'Fixed disk')
}

BPB_FMT_VAL_OFFSET  = 0
BPB_FMT_VAL_LEN     = 1
BPB_FMT_VAL_STIDX   = 2

bpb_format = {
# Field                     Offset Length  fieldIdx
# -----                     ------ ------ ----------
    'JmpBoot'       : (0x00, 3,0) ,
    'OEMName'       : (0x04, 8,1) ,
    'BytsPerSec'    : (0x0b, 2,2) , 
    'SecPerClus'    : (0x0d, 1,3) , 
    'RsvdSecs'      : (0x0e, 2,4) , 
    'NumFATs'       : (0x10, 1,5) ,
    'RootEntCnt'    : (0x11, 2,6) ,
    'TotSec16'      : (0x13, 2,7) ,
    'MediaDesp'     : (0x15, 1,8) ,
    'SecPerFAT16'   : (0x16, 2,9) ,         #secPerFAT16
# DOS 3.31 ---               
    'SecPerTrk'     : (0x18, 2,10) ,
    'NumHeads'      : (0x1a, 2,11) ,
    'HidenSec'      : (0x1c, 4,12) ,
    'TotSec32'      : (0x20, 4,13)
}

basic_bpb_format = '<3s8sHBHBHHBHHHII'      #jmp asm code + oemID + bpb	
bpblen = sum( [i[1] for i in bpb_format.values()] )

FAT16ebpb_format = {
# ...DOS 4.0 ---
# Field                     Offset Length
# -----                     ------ ------
    'PhyDrvNum'     : (0x24, 1,14) ,
    'Rsvd1'         : (0x25, 1,15) ,
    'BootSig'       : (0x26, 1,16) ,
    'VolID'         : (0x27, 4,17) ,
    'VolLabel'      : (0x2b, 11,18) ,
    'FilSysType'    : (0x36, 8,19) 			# type for fat12/16
}
for k in FAT16ebpb_format.keys():
    print 'KEY_BPB_EXT16_' + k + ' = ' + '\'' + k + '\''

FAT16ebpbfields = 'BBBI11s8s'
FAT16ebpblen = sum( [i[1] for i in FAT16ebpb_format.values()] )

FAT32ebpb_format = {
# relative offsets
    'SecPerFAT32'   : (0x24, 4,14) ,
    'DrvDesp'       : (0x28, 2,15) ,
    'FSVer'         : (0x2a, 2,16) ,
    'RootClus'      : (0x2c, 4,17) ,    # typically 2 ,
    'FSInfo' 	    : (0x30, 2,18) ,	# typically 1 ,
    'BkBootSec'     : (0x32, 2,19) ,
    'Rsvd2'         : (0x34, 12,20),

# ...same as DOS 3.31/DOS 4.0 ebpb...
    'PhyDrvNum'     : (0x40, 1,21) ,
    'Rsvd1'         : (0x41, 1,22) ,
    'BootSig'       : (0x42, 1,23) ,
    'VolID'         : (0x43, 4,24) ,
    'VolLabel'      : (0x47, 11,25) ,
    'FilSysType'    : (0x53, 8,26)
}
for k in FAT32ebpb_format.keys():
    print 'KEY_BPB_EXT32_' + k + ' = ' + '\'' + k + '\''

FAT32ebpbfields = 'IHHIHH12sBBBI11s8s'
FAT32ebpblen = sum( [i[1] for i in FAT32ebpb_format.values()] )

signature = 'BB'				#for 55aa 	

# How much space for boot code?
FAT16bootcodelength = 512 \
		    - struct.calcsize(basic_bpb_format) \
		    - struct.calcsize(signature)	\
		    - struct.calcsize(FAT16ebpbfields)

FAT32bootcodelength = 512 \
		    - struct.calcsize(basic_bpb_format) \
		    - struct.calcsize(signature)	\
		    - struct.calcsize(FAT32ebpbfields)

# Finally, set up the boot-sector formats
FAT16_middle = FAT16ebpbfields + '%ds'%FAT16bootcodelength
FAT16boot_sector_format = basic_bpb_format + FAT16_middle + signature

FAT32_middle = FAT32ebpbfields + '%ds'%FAT32bootcodelength
FAT32boot_sector_format = basic_bpb_format + FAT32_middle + signature

def FATtype(vbr):
    '''Identify the FAT type based on volume boot record'''
    # vbr[bpb_format[KEY_BPB_EXT16_FilSysType][BPB_FMT_VAL_OFFSET]].startwiths('FAT12')

    if clustercount(fields) < 4087:
        return 'FAT12'
    elif clustercount(fields) < 65527:
        return 'FAT16'
    elif clustercount(fields) < 268435447:
        return 'FAT32'
    else:
        return 'FAT'

def get_boot_sector_bytes(filename):
    '''Open a file, read the boot sector;
    return the file handle and the boot-sector bytes.
    '''
    f = open(filename)
    b = f.read(512)
    return (f,b)
#----

def get_boot_sector(filename):
    '''Open a file, read the boot sector, parse it;
    return the file handle, the boot-sector bytes,
    and the boot-sector fields interpreted as both FAT12/16 and FAT32.
    '''
    f,b = get_boot_sector_bytes(filename)
    f16fields = struct.unpack(FAT16boot_sector_format, b)
    f32fields = struct.unpack(FAT32boot_sector_format, b)
    return (f, b, f16fields, f32fields)
#----

def clustercount(fields):
    '''Calculate the number of clusters identified in the boot sector.'''
    if fields[sectors_per_FAT] != 0:
        n_FAT_sectors = fields[sectors_per_FAT]
    else:
        n_FAT_sectors = fields[sectors_per_FAT32]

    root_dir_start = fields[n_reserved_sectors] * fields[bytes_per_sector] \
        + n_FAT_sectors * fields[n_FATs]

    bytes_in_root_dir = fields[n_root_entries] * 32
    data_start_bytes = root_dir_start + bytes_in_root_dir
    data_start_sectors = data_start_bytes / fields[bytes_per_sector]

    if fields[n_sectors_small] != 0:
        n_sectors = fields[n_sectors_small]
    else:
        n_sectors = fields[n_sectors_large]

    n_clusters = (n_sectors - data_start_sectors) / fields[sectors_per_cluster]
    return n_clusters
#----

