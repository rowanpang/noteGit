#!/usr/bin/python
# -*- coding: utf-8 -*-

import yum
import os

yb = yum.YumBase()
_conf = yb._getConfig()

print yb.arch.basearch
print yb.arch.canonarch
print _conf.max_connections
print _conf.yumvar
print yb.conf.yumvar
print yb.conf.yumvar['basearch']

from rpmUtils.arch import archDifference, canCoinstall, ArchStorage, isMultiLibArch
myarch = ArchStorage()

print myarch.basearch

print os.uname()[4]

