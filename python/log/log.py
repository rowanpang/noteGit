#!/usr/bin/python
# -*- coding: utf-8 -*-

import logging
import logging.handlers

# logging.basicConfig(filename='example.log',level=logging.DEBUG)
fmt = None
fmt = "%(asctime)s,%(levelname)8s,%(name)8s: %(message)s"
fmtTime = "%Y-%m-%d %H-%M-%S"
print logging.getLevelName(logging.root.getEffectiveLevel())
logging.basicConfig(level=logging.DEBUG,format=fmt,datefmt=fmtTime)
print logging.getLevelName(logging.root.getEffectiveLevel())

logging.debug('This message should go to the log file')
logging.info('So should this')
logging.warning('And this, too')

fmter = logging.Formatter(fmt=fmt,datefmt=fmtTime)

logger = logging.getLogger('ihm')
logger.setLevel(logging.DEBUG)
logger.propagate = 0

hdleStream = logging.StreamHandler()
hdleStream.setFormatter(fmter)
hdleFile = logging.FileHandler('ihm.log')
hdleFile.setFormatter(fmter)
logger.addHandler(hdleStream)
logger.addHandler(hdleFile)
logger.debug('1.sssss.sss')

logger = logging.getLogger('ihm.a')
hdleStream = logging.StreamHandler()
hdleStream.setFormatter(fmter)
logger.addHandler(hdleStream)
logger.debug('2.sssss.sss')
    #这个msg 将会打印2次,logger ihm.a 以及ihm 的handler都会处理.


