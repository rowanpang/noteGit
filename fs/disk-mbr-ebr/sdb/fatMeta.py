#!/usr/bin/env python
# Parse the initial bytes from a FAT-fs image.
# 2012-04-08

import struct
import binascii
import hashlib
import sys

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
    
    FAT_CLUS_ST_FREE        = 0x01
    FAT_CLUS_ST_RESERVED    = 0x02
    FAT_CLUS_ST_DEFECTIVE   = 0x04
    FAT_CLUS_ST_INVALID_MASK= 0x07

    FAT_CLUS_ST_ALLOCATED   = 0x08
    FAT_CLUS_ST_FINAL       = 0x10

    FAT_CLUS_ST_IDFY_12_FREE            = 0x000
    FAT_CLUS_ST_IDFY_12_ALLOCATED_LOW   = 0x002
    FAT_CLUS_ST_IDFY_12_ALLOCATED_HIGH  = 0xFEF
    FAT_CLUS_ST_IDFY_12_DEFECTIVE       = 0xFF7
    FAT_CLUS_ST_IDFY_12_FINAL_START     = 0xFF8

    FAT_CLUS_ST_IDFY_16_FREE            = 0x0001
    FAT_CLUS_ST_IDFY_16_ALLOCATED_LOW   = 0x0002
    FAT_CLUS_ST_IDFY_16_ALLOCATED_HIGH  = 0xFFEF
    FAT_CLUS_ST_IDFY_16_DEFECTIVE       = 0xFFF7
    FAT_CLUS_ST_IDFY_16_FINAL_START     = 0xFFF8

    FAT_CLUS_ST_IDFY_32_FREE            = 0x00000000
    FAT_CLUS_ST_IDFY_32_ALLOCATED_LOW   = 0x00000002
    FAT_CLUS_ST_IDFY_32_ALLOCATED_HIGH  = 0x0FFFFFEF
    FAT_CLUS_ST_IDFY_32_DEFECTIVE       = 0x0FFFFFF7
    FAT_CLUS_ST_IDFY_32_FINAL_START     = 0x0FFFFFF8

    FAT_ENT_SIZE = 32               #long short entry both are 32 bytes.
    FAT_ENT_ATTR_OFF = 11

    FAT_ENT_ATTR_READ_ONLY  = 0x01
    FAT_ENT_ATTR_HIDDEN     = 0x02
    FAT_ENT_ATTR_SYSTEM     = 0x04
    FAT_ENT_ATTR_VOLUME_ID  = 0x08

    FAT_ENT_ATTR_DIRECOTRY  = 0x10
    FAT_ENT_ATTR_ARCHIVE    = 0x20

    FAT_ENT_ATTR_LONGNAME   = FAT_ENT_ATTR_READ_ONLY |  \
                              FAT_ENT_ATTR_HIDDEN    |  \
                              FAT_ENT_ATTR_SYSTEM    |  \
                              FAT_ENT_ATTR_VOLUME_ID


    FAT_ENT_KEY_short_name          = 'name'
    FAT_ENT_KEY_short_ext           = 'ext'
    FAT_ENT_KEY_short_attr          = 'attr'
    FAT_ENT_KEY_short_NTRes         = 'NTRes'
    FAT_ENT_KEY_short_crtTimeMS     = 'crtTimeMS'
    FAT_ENT_KEY_short_crtTime       = 'crtTime'
    FAT_ENT_KEY_short_crtDate       = 'crtDate'
    FAT_ENT_KEY_short_lastAccData   = 'lastAccData'
    FAT_ENT_KEY_short_FstClusHI     = 'FstClusHI'
    FAT_ENT_KEY_short_wrtTime       = 'wrtTime'
    FAT_ENT_KEY_short_wrtDate       = 'wrtDate'
    FAT_ENT_KEY_short_FstClusLO     = 'FstClusLO'
    FAT_ENT_KEY_short_FileSize      = 'FileSize'

    FAT_ENT_KEY_long_ord        = 'ord'
    FAT_ENT_KEY_long_name1      = 'name1'
    FAT_ENT_KEY_long_attr       = 'attr'
    FAT_ENT_KEY_long_Type       = 'Type'
    FAT_ENT_KEY_long_chksum     = 'chksum'
    FAT_ENT_KEY_long_name2      = 'name2'
    FAT_ENT_KEY_long_FstClusLO  = 'FstClusLO'
    FAT_ENT_KEY_long_name3      = 'name3'

    FAT_ENT_FMT_VAL_OFFSET  = 0
    FAT_ENT_FMT_VAL_LEN     = 1
    FAT_ENT_FMT_VAL_STIDX   = 2

    FAT_ENT_LONG_LAST       = 0x40
    FAT_ENT_LONG_NUM_MASK   = 0x1f

    shortEnt_format = {
        'name'        : (0x00, 8,0) ,
        'ext'         : (0x08, 3,1) ,
        'attr'        : (0x0b, 1,2) , 
        'NTRes'       : (0x0c, 1,3) , 
        'crtTimeMS'   : (0x0d, 1,4) , 
        'crtTime'     : (0x0e, 2,5) ,
        'crtDate'     : (0x10, 2,6) ,
        'lastAccData' : (0x12, 2,7) ,
        'FstClusHI'   : (0x14, 2,8) ,
        'wrtTime'     : (0x16, 2,9) ,         #secPerFAT16
        'wrtDate'     : (0x18, 2,10) ,         #secPerFAT16
        'FstClusLO'   : (0x1a, 2,11) ,
        'FileSize'    : (0x1c, 4,12) ,
    }
    shortEnt_fields = '=8s3sBBBHHHHHHHI'      #jmp asm code + oemID + bpb	

    longEnt_format = {
        'ord'       : (0x00, 1,0) ,
        'name1'     : (0x01, 10,1) ,
        'attr'      : (0x0b, 1,2) , 
        'Type'      : (0x0c, 1,3) , 
        'chksum'    : (0x0d, 1,4) , 
        'name2'     : (0x0e, 12,5) ,
        'FstClusLO' : (0x1a, 2,6) ,
        'name3'     : (0x1c, 4,7) ,
    }
    longEnt_fields = '=B10sBBB12sH4s'      #jmp asm code + oemID + bpb	

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

    def get_clusteridx_max(self):
        st = self.get_vbr_fields();
        totSec = st[self.bpb_format[self.KEY_BPB_BASE_TotSec32][self.BPB_FMT_VAL_STIDX]]
        secPerClus = st[self.bpb_format[self.KEY_BPB_BASE_SecPerClus][self.BPB_FMT_VAL_STIDX]]

        numMax = (totSec - self.get_dataStartSec()) / secPerClus
        fsType = self.get_type()
        if fsType == self.FAT_TYPE_12:
            if numMax > self.FAT_CLUS_ST_IDFY_12_ALLOCATED_HIGH:
                numMax = self.FAT_CLUS_ST_IDFY_12_ALLOCATED_HIGH
        elif fsType == self.FAT_TYPE_16:
            if numMax > self.FAT_CLUS_ST_IDFY_16_ALLOCATED_HIGH:
                numMax = self.FAT_CLUS_ST_IDFY_16_ALLOCATED_HIGH
        elif fsType == self.FAT_TYPE_32:
            if numMax > self.FAT_CLUS_ST_IDFY_32_ALLOCATED_HIGH:
                numMax = self.FAT_CLUS_ST_IDFY_32_ALLOCATED_HIGH

    def get_cluster_status(self,clusVal):
        fsType = self.get_type()
        if fsType == self.FAT_TYPE_12:
            if clusVal == self.FAT_CLUS_ST_IDFY_12_FREE:
                return self.FAT_CLUS_ST_FREE
            elif (clusVal >= self.FAT_CLUS_ST_IDFY_12_ALLOCATED_LOW) and (clusVal <= self.get_clusteridx_max()):
                return self.FAT_CLUS_ST_ALLOCATED
            elif clusVal == self.FAT_CLUS_ST_IDFY_12_DEFECTIVE:
                return self.FAT_CLUS_ST_DEFECTIVE
            elif clusVal >= self.FAT_CLUS_ST_IDFY_12_FINAL_START:
                return self.FAT_CLUS_ST_FINAL

        elif fsType == self.FAT_TYPE_16:
            if clusVal == self.FAT_CLUS_ST_IDFY_16_FREE:
                return self.FAT_CLUS_ST_FREE
            elif (clusVal >= self.FAT_CLUS_ST_IDFY_16_ALLOCATED_LOW) and (clusVal <= self.get_clusteridx_max()):
                return self.FAT_CLUS_ST_ALLOCATED
            elif clusVal == self.FAT_CLUS_ST_IDFY_16_DEFECTIVE:
                return self.FAT_CLUS_ST_DEFECTIVE
            elif clusVal >= self.FAT_CLUS_ST_IDFY_16_FINAL_START:
                return self.FAT_CLUS_ST_FINAL

        elif fsType == self.FAT_TYPE_32:
            if clusVal == self.FAT_CLUS_ST_IDFY_32_FREE:
                return self.FAT_CLUS_ST_FREE
            elif (clusVal >= self.FAT_CLUS_ST_IDFY_32_ALLOCATED_LOW) and (clusVal <= self.get_clusteridx_max()):
                return self.FAT_CLUS_ST_ALLOCATED
            elif clusVal == self.FAT_CLUS_ST_IDFY_32_DEFECTIVE:
                return self.FAT_CLUS_ST_DEFECTIVE
            elif clusVal >= self.FAT_CLUS_ST_IDFY_32_FINAL_START:
                return self.FAT_CLUS_ST_FINAL

        return self.FAT_CLUS_ST_RESERVED

    def get_cluster_next(self,clusidx):        #both are little endian  
        fat = self.get_FAT()
        fsType = self.get_type()
        if fsType == self.FAT_TYPE_12:
            bytsoff = (clusidx * 3) >> 1
            blen = 2
            bformat='<H'
            clusVal = struct.unpack(bformat,fat[bytsoff:bytsoff+blen])
            if bytsoff & 1 == 0:
                clusVal &= 0xfff
            else:
                clusVal >= 4
            
        elif fsType == self.FAT_TYPE_16:
            bytsoff = clusidx * self.FAT_CLUS_BYTES_16
            blen = 2
            bformat='<H'
            clusVal = struct.unpack(bformat,fat[bytsoff:bytsoff+blen])

        elif fsType == self.FAT_TYPE_32:
            bytsoff = clusidx * self.FAT_CLUS_BYTES_32
            blen = 4
            bformat='<I'
            clusVal = struct.unpack(bformat,fat[bytsoff:bytsoff+blen])

        return clusVal[0]
    
    def read_cluster(self,clusidx):
        st = self.get_vbr_fields();
        clusSt = self.get_cluster_status(clusidx)
        # print('clusidx:%#x,clusSt:%#x' %(clusidx,clusSt))
        if not (clusSt & self.FAT_CLUS_ST_ALLOCATED):           
            return None

        if self.isFAT12or16():
            clusStart = 2
        elif self.isFAT32():
            clusStart = st[self.FAT32ebpb_format[self.KEY_BPB_EXT32_RootClus][self.BPB_FMT_VAL_STIDX]]

        secPerClus = st[self.bpb_format[self.KEY_BPB_BASE_SecPerClus][self.BPB_FMT_VAL_STIDX]]

        secOff = (clusidx-clusStart) * secPerClus 
        secStart = self.get_dataStartSec() + secOff         #rootdir same as data start.
        return self.read_sector(secPerClus,secStart)

    def read_cluster_chain(self,clusStart,num=0):
        '''if num == 0,read the whole cluster chain '''
        if num == 0:
            num = sys.maxint

        i = 0
        b = self.read_cluster(clusStart)
        if not b:
            return None,i
        i += 1
        if i == num:
            return b,i

        while True:
            clusStart = self.get_cluster_next(clusStart)
            clusSt = self.get_cluster_status(clusStart)
            if clusSt & self.FAT_CLUS_ST_ALLOCATED:
                tmpbuf = self.read_cluster(clusStart)
                b += tmpbuf
                i += 1
                if i == num:
                    return b,i
            else:
                if clusSt & self.FAT_CLUS_ST_FINAL:
                    i = 0
                break;

        return b,i

    def get_rootdir_content(self):
        st = self.get_vbr_fields();
        if self.isFAT12or16():
            numEnt = st[self.bpb_format[self.KEY_BPB_BASE_RootEntCnt][self.BPB_FMT_VAL_STIDX]] 
            return self.read_bypte_secAlign(numEnt*self.FAT_ENT_SIZE,self.get_dataStartSec())
        elif self.isFAT32():
            curClus = st[self.FAT32ebpb_format[self.KEY_BPB_EXT32_RootClus][self.BPB_FMT_VAL_STIDX]]
            return self.read_cluster_chain(curClus)[0]

    def entry_read(self,dirbuf,idx,nums):
        return dirbuf[idx*self.FAT_ENT_SIZE:(idx+nums)*self.FAT_ENT_SIZE]
   
    def entry_parser_short(self,ent):
        etfield = struct.unpack(self.shortEnt_fields,ent)

        entry = {}
        for k in self.shortEnt_format.keys():
            entry[k] = etfield[self.shortEnt_format[k][self.FAT_ENT_FMT_VAL_STIDX]]

        return entry

    def entry_parser_longShort(self,entrys):
        nums = len(entrys)/self.FAT_ENT_SIZE


    def list_root(self):
        rb = self.get_rootdir_content()
        for idx in range(0,len(rb)/self.FAT_ENT_SIZE):
            ent = self.entry_read(rb,idx,1)
            if ent[0] == '\x00' or ent[0] == '\xe5':
                print '-----entry---end-----'
                break
            elif ent[0] == '\x05':
                ent.replace('\x05','\xe5',1)

            attr = ord(ent[self.FAT_ENT_ATTR_OFF:self.FAT_ENT_ATTR_OFF+1])
            print('%x' %(attr))
            if attr & self.FAT_ENT_ATTR_LONGNAME:
                totEnt = ord(ent[self.longEnt_format[self.FAT_ENT_KEY_long_ord][self.FAT_ENT_FMT_VAL_STIDX]])
                if totEnt & self.FAT_ENT_LONG_LAST:
                    totEnt &= self.FAT_ENT_LONG_NUM_MASK
                    entslong = ent + self.entry_read(rb,idx+1,totEnt)
                    idx += totEnt

                else:
                    TypeError
            else:
                print self.entry_parser_short(ent)
        
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
