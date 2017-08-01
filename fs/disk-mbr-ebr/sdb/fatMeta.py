#!/usr/bin/env python
# Parse the initial bytes from a FAT-fs image.
# 2012-04-08

import struct
import binascii
import hashlib

# formats of various parts of a FAT boot sector:

class FatTypeError(Exception):
    def __init__(self,x='not a fat filesystem'):
        Exception.__init__(self,x)

class FileAssociateTableError(Exception):
    def __init__(self,x='File associate table check error'):
        Exception.__init__(self,x)

class fatVBR:
    VBR_SIZE_IN_BYTE = 512

    FAT_CLUS_BYTES_12 = 3/2 
    FAT_CLUS_BYTES_16 = 2
    FAT_CLUS_BYTES_32 = 4

    FAT_ENT_SIZE = 32               #long short entry both are 32 bytes.

    FAT_TYPE_12_IDFY_STR='FAT12'
    FAT_TYPE_16_IDFY_STR='FAT16'
    FAT_TYPE_32_IDFY_STR='FAT32'
    FAT_TYPE_12=0
    FAT_TYPE_16=1
    FAT_TYPE_32=2
    FAT_TYPE_NONE=0xFF

    KEY_BPB_BASE_JmpBoot            = 'JmpBoot'
    KEY_BPB_BASE_OEMName            = 'OEMName'
    KEY_BPB_BASE_BytsPerSec	    = 'BytsPerSec'
    KEY_BPB_BASE_SecPerClus	    = 'SecPerClus'
    KEY_BPB_BASE_RsvdSecs	    = 'RsvdSecs'
    KEY_BPB_BASE_NumFATs	    = 'NumFATs'
    KEY_BPB_BASE_RootEntCnt	    = 'RootEntCnt'
    KEY_BPB_BASE_TotSec16	    = 'TotSec16'
    KEY_BPB_BASE_MediaDesp	    = 'MediaDesp'
    KEY_BPB_BASE_SecPerFAT16        = 'SecPerFAT16'
    KEY_BPB_BASE_SecPerTrk	    = 'SecPerTrk'
    KEY_BPB_BASE_NumHeads	    = 'NumHeads'
    KEY_BPB_BASE_HidenSec	    = 'HidenSec'
    KEY_BPB_BASE_TotSec32	    = 'TotSec32'

    KEY_BPB_EXT16_PhyDrvNum	    = 'PhyDrvNum'
    KEY_BPB_EXT16_Rsvd1	            = 'Rsvd1'
    KEY_BPB_EXT16_BootSig	    = 'BootSig'
    KEY_BPB_EXT16_VolID	            = 'VolID'
    KEY_BPB_EXT16_VolLabel	    = 'VolLabel'
    KEY_BPB_EXT16_FilSysType        = 'FilSysType'

    KEY_BPB_EXT32_SecPerFAT32       = 'SecPerFAT32'
    KEY_BPB_EXT32_DrvDesp	    = 'DrvDesp'
    KEY_BPB_EXT32_FSVer	            = 'FSVer'
    KEY_BPB_EXT32_RootClus	    = 'RootClus'
    KEY_BPB_EXT32_FSInfo	    = 'FSInfo'
    KEY_BPB_EXT32_BkBootSec	    = 'BkBootSec'
    KEY_BPB_EXT32_Rsvd2	            = 'Rsvd2'
    KEY_BPB_EXT32_PhyDrvNum	    = 'PhyDrvNum'
    KEY_BPB_EXT32_Rsvd1	            = 'Rsvd1'
    KEY_BPB_EXT32_BootSig	    = 'BootSig'
    KEY_BPB_EXT32_VolID	            = 'VolID'
    KEY_BPB_EXT32_VolLabel	    = 'VolLabel'
    KEY_BPB_EXT32_FilSysType        = 'FilSysType'

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

    # Field      :(Offset, Length, fieldIdx)
    BPB_FMT_VAL_OFFSET  = 0
    BPB_FMT_VAL_LEN     = 1
    BPB_FMT_VAL_STIDX   = 2

    bpb_format = {
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
    basicbpbfields = '=3s8sHBHBHHBHHHII'      #jmp asm code + oemID + bpb	
    basicbpblen = sum( [i[1] for i in bpb_format.values()] )

    FAT16ebpb_format = {
        # ...DOS 4.0 ---
        'PhyDrvNum'     : (0x24, 1,14) ,
        'Rsvd1'         : (0x25, 1,15) ,
        'BootSig'       : (0x26, 1,16) ,
        'VolID'         : (0x27, 4,17) ,
        'VolLabel'      : (0x2b, 11,18) ,
        'FilSysType'    : (0x36, 8,19) 			# type for fat12/16
    }
    # case of aligment ,can not use struct.calcsize to calculate size
    FAT16ebpbfields = 'BBBI11s8s'
    FAT16ebpblen = sum( [i[1] for i in FAT16ebpb_format.values()] )

    FAT32ebpb_format = {
        'SecPerFAT32'   : (0x24, 4,14) ,
        'DrvDesp'       : (0x28, 2,15) ,
        'FSVer'         : (0x2a, 2,16) ,
        'RootClus'      : (0x2c, 4,17) ,    # typically 2 ,
        'FSInfo' 	: (0x30, 2,18) ,    # typically 1 ,
        'BkBootSec'     : (0x32, 2,19) ,
        'Rsvd2'         : (0x34, 12,20),
        # ...same as DOS 3.31/DOS 4.0 ebpb...
        'PhyDrvNum'     : (0x40, 1,21) ,
        'Rsvd1'         : (0x41, 1,22) ,
        'BootSig'       : (0x42, 1,23) ,
        'VolID'         : (0x43, 4,24) ,
        'VolLabel'      : (0x47, 11,25) ,
        'FilSysType'    : (0x52, 8,26)
    }
    FAT32ebpbfields = 'IHHIHH12sBBBI11s8s'
    FAT32ebpblen = sum( [i[1] for i in FAT32ebpb_format.values()] )

    signature = 'BB'				#for 55aa 	

    # How much space for boot code?
    FAT16bootcodelength = VBR_SIZE_IN_BYTE  \
                        - basicbpblen       \
                        - FAT16ebpblen      \
                        - struct.calcsize(signature)

    FAT32bootcodelength = VBR_SIZE_IN_BYTE  \
                        - basicbpblen       \
                        - FAT32ebpblen      \
                        - struct.calcsize(signature)

    # Finally, set up the boot-sector formats
    FAT16_middle = FAT16ebpbfields + '%ds'%FAT16bootcodelength
    FAT16vbr_format = basicbpbfields + FAT16_middle + signature

    FAT32_middle = FAT32ebpbfields + '%ds'%FAT32bootcodelength
    FAT32vbr_format = basicbpbfields + FAT32_middle + signature

    def __init__(self,filename):
        self.filename = filename
        self.vbr = ''
        self.fsType = None
        self.fields = None
        self.fmt = None
        self.FAT = None

        f = open(self.filename)
        self.vbr = f.read(self.VBR_SIZE_IN_BYTE)
        f.close()

        idfyStr = self.vbr[self.FAT16ebpb_format[self.KEY_BPB_EXT16_FilSysType][self.BPB_FMT_VAL_OFFSET]:]
        # print idfyStr[:6]
        if idfyStr.startswith(self.FAT_TYPE_12_IDFY_STR):
            self.fsType = self.FAT_TYPE_12
        elif idfyStr.startswith(self.FAT_TYPE_16_IDFY_STR):
            self.fsType = self.FAT_TYPE_16
        else:
            idfyStr = self.vbr[self.FAT32ebpb_format[self.KEY_BPB_EXT32_FilSysType][self.BPB_FMT_VAL_OFFSET]:]
            # print idfyStr[:6]
            if idfyStr.startswith(self.FAT_TYPE_32_IDFY_STR):
                self.fsType = self.FAT_TYPE_32
            else:
                self.fsType = self.FAT_TYPE_NONE
                raise FatTypeError

    def get_type(self):
        '''Identify the FAT type based on volume boot record'''
        return self.fsType

    def isFAT12or16(self):
        fsType = self.get_type()
        if fsType == self.FAT_TYPE_12 or fsType == self.FAT_TYPE_16:
            return True
        else:
            return False

    def isFAT32(self):
        fsType = self.get_type()
        if fsType == self.FAT_TYPE_32:
            return True
        else:
            return False

    def get_vbr_bytes(self):
        '''Open a file, read the boot sector;
            return the boot-sector bytes.
        '''
        return self.vbr

    def get_vbr_fields(self):
        '''Open a file, read the boot sector, parse it;
            return the boot-sector fields interpreted as FATxx
        '''

        if self.fields == None:
            if self.isFAT12or16():
                self.fields = struct.unpack(self.FAT16vbr_format, self.vbr)
            elif self.isFAT32():
                self.fields = struct.unpack(self.FAT32vbr_format, self.vbr)
            return self.fields
        else:
            return self.fields
    
    def read_bytes(self,size,seek=0):
        with open(self.filename) as f:
            f.seek(seek)
            return f.read(size)

    def read_bypte_secAlign(self,size,seekSec=0):
        bytsPerSec = self.get_vbr_fields()[self.bpb_format[self.KEY_BPB_BASE_BytsPerSec][self.BPB_FMT_VAL_STIDX]]
        start = seekSec * bytsPerSec
        return self.read_bytes(size,start)

    def read_sector(self,sizeSec,seekSec=0):
        bytsPerSec = self.get_vbr_fields()[self.bpb_format[self.KEY_BPB_BASE_BytsPerSec][self.BPB_FMT_VAL_STIDX]]

        size = sizeSec * bytsPerSec
        seek = seekSec * bytsPerSec
        return self.read_bytes(size,seek)
        
    def get_FAT_secStart(self):
        st = self.get_vbr_fields();
        return st[self.bpb_format[self.KEY_BPB_BASE_RsvdSecs][self.BPB_FMT_VAL_STIDX]]

    def get_FAT(self):
        if self.FAT == None:
            st = self.get_vbr_fields();
            secRsvd = st[self.bpb_format[self.KEY_BPB_BASE_RsvdSecs][self.BPB_FMT_VAL_STIDX]]
            numFAT = st[self.bpb_format[self.KEY_BPB_BASE_NumFATs][self.BPB_FMT_VAL_STIDX]]

            if self.isFAT12or16():
                secPerFAT = st[self.bpb_format[self.KEY_BPB_BASE_SecPerFAT16][self.BPB_FMT_VAL_STIDX]]
            elif self.isFAT32():
                secPerFAT = st[self.FAT32ebpb_format[self.KEY_BPB_EXT32_SecPerFAT32][self.BPB_FMT_VAL_STIDX]]

            fat1 = self.read_sector(secPerFAT,secRsvd)
            if numFAT > 1:
                fat2 = self.read_sector(secPerFAT,secRsvd+secPerFAT)

                hasher1 = hashlib.sha1()
                hasher1.update(fat1)

                hasher2 = hashlib.sha1()
                hasher2.update(fat2)

                if hasher1.digest() == hasher2.digest():
                    self.FAT = fat1
                else:
                    raise FileAssociateTableError
            else:
                self.FAT = fat1

            return self.FAT
        else:
            return self.FAT

    def get_dataStartSec(self):
        st = self.get_vbr_fields();
        secRsvd = st[self.bpb_format[self.KEY_BPB_BASE_RsvdSecs][self.BPB_FMT_VAL_STIDX]]

        if self.isFAT12or16():
            secPerFAT = st[self.bpb_format[self.KEY_BPB_BASE_SecPerFAT16][self.BPB_FMT_VAL_STIDX]]
        elif self.isFAT32():
            secPerFAT = st[self.FAT32ebpb_format[self.KEY_BPB_EXT32_SecPerFAT32][self.BPB_FMT_VAL_STIDX]]

        numFAT = st[self.bpb_format[self.KEY_BPB_BASE_NumFATs][self.BPB_FMT_VAL_STIDX]]

        return secRsvd + numFAT * secPerFAT
    
    def read_cluster(self,clusidx):
        st = self.get_vbr_fields();

        if self.isFAT12or16():
            clusStart = 2
        elif self.isFAT32():
            clusStart = st[self.FAT32ebpb_format[self.KEY_BPB_EXT32_RootClus][self.BPB_FMT_VAL_STIDX]]

        secPerClus = st[self.bpb_format[self.KEY_BPB_BASE_SecPerClus][self.BPB_FMT_VAL_STIDX]]
        secOff = (clusidx - clusStart) * secPerClus 
        secStart = self.get_dataStartSec() + secOff         #rootdir same as data start.

        return self.read_sector(secPerClus,secStart)

    def get_cluster_status(self,clusidx):
        fat = self.get_FAT()
        if self.isFAT12or16():
            clusStart = 2
        elif self.isFAT32():
            clusStart = st[self.FAT32ebpb_format[self.KEY_BPB_EXT32_RootClus][self.BPB_FMT_VAL_STIDX]]

        clusoff = clusidx - clusStart
        
    def get_rootdir_content(self):
        st = self.get_vbr_fields();
        if self.isFAT12or16():
            numEnt = st[self.bpb_format[self.KEY_BPB_BASE_RootEntCnt][self.BPB_FMT_VAL_STIDX]] 
            return self.read_bypte_secAlign(numEnt*self.FAT_ENT_SIZE,self.get_dataStartSec())
        elif self.isFAT32():
            curClus = st[self.FAT32ebpb_format[self.KEY_BPB_EXT32_RootClus][self.BPB_FMT_VAL_STIDX]]
            return self.read_cluster(curClus)

    def list_root(self):
        rb = self.get_rootdir_content()
        

    def get_fmt(self):
        if self.fmt == None:
            fmt1 = self.bpb_format
            if self.isFAT12or16():
                fmt2 = self.FAT16ebpb_format
            elif self.isFAT32():
                fmt2 = self.FAT32ebpb_format

            self.fmt = fmt1.copy()
            self.fmt.update(fmt2)
            return self.fmt
        else:
            return self.fmt

    def parser_fields(self):
        st = self.get_vbr_fields();
        fmt = self.get_fmt()
        klen = 0
        for k in fmt.keys():
            if len(k) > klen:
                klen = len(k)
        prfmt = '%'+'%d' %(klen)+'s: %s' 
        for k in fmt.keys():
            print(prfmt %(k,st[fmt[k][self.BPB_FMT_VAL_STIDX]]))

if __name__ == '__main__':
    fp = fatVBR('/dev/sdb1')
    # fp = fatVBR('./vbr-fat16.bin')
    print fp.get_FAT_secStart()
    print fp.get_dataStartSec()
    fp.parser_fields()

    print binascii.hexlify(fp.get_rootdir_content())
