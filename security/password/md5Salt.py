#!/usr/bin/python
# -*- coding: utf-8 -*-

from random import Random
from hashlib import md5

# 获取由4位随机大小写字母、数字组成的salt值
def create_salt(length = 5):
    salt = ''
    chars = 'AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789'
    len_chars = len(chars) - 1
    random = Random()
    for i in xrange(length):
        # 每次从chars中随机取一位
        salt += chars[random.randint(0,len_chars)]
    return salt

# 获取原始密码+salt的md5值
def create_md5(pwd,salt):
    md5_obj = md5()
    md5_obj.update(pwd + salt)
    return md5_obj.hexdigest()

# 原始密码
pwd = 'inspur123'
# 随机生成4位salt
salt = create_salt(10)
# 加密后的密码
md5 = create_md5(pwd, salt)

print '[pwd]\n',pwd
print '[salt]\n', salt
print '[md5]\n', md5
