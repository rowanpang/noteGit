#!/usr/bin/python
import os
import binascii
import hashlib

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.ciphers import (
    Cipher, algorithms, modes,BlockCipherAlgorithm
)
from cryptography.hazmat.primitives import padding

cached_keys= {}
def EVP_BytesToKey(password, key_len, iv_len):
    # equivalent to OpenSSL's EVP_BytesToKey() with count 1
    # so that we make the same key and iv as nodejs version
    cached_key = '%s-%d-%d' % (password, key_len, iv_len)
    r = cached_keys.get(cached_key, None)
    if r:
        return r
    m = []
    i = 0
    while len(b''.join(m)) < (key_len + iv_len):
        md5 = hashlib.md5()
        data = password
        if i > 0:
            data = m[i - 1] + password
        md5.update(data)
        m.append(md5.digest())
        i += 1
    ms = b''.join(m)
    key = ms[:key_len]
    iv = ms[key_len:key_len + iv_len]
    cached_keys[cached_key] = (key, iv)
    return key, iv

def aesMode(pwd,mod):
    print 'mode:',mod
    if mod == 'cbc':
        return aes256cbc(pwd)
    elif mod == 'cfb':
        return aes256cfb(pwd)
    else:
        return None

def aes256cfb(pwd):
    key,iv = EVP_BytesToKey(pwd,32,16)
    algorithm = algorithms.AES(key)
    mode = modes.CFB(iv)

    return Cipher(algorithm, mode, backend=default_backend())

def aes256cbc(pwd):
    key,iv = EVP_BytesToKey(pwd,32,16)
    algorithm = algorithms.AES(key)
    mode = modes.CBC(iv)

    return Cipher(algorithm, mode, backend=default_backend())

def decrypt(cipher,cipherHex):
    decryptor = cipher.decryptor()
    plain = decryptor.update(cipherHex) + decryptor.finalize()
    print 'plain with padding:',binascii.hexlify(plain)
    unpadder = padding.PKCS7(cipher.algorithm.block_size).unpadder()
    plain = unpadder.update(plain)
    plain += unpadder.finalize()

    return plain

def encrypt(cipher,plaintxt):
    # if (len(plaintxt)*8)%blockSize != 0:
    print '---alway do padding for we also need unpadding check---'
    padder = padding.PKCS7(cipher.algorithm.block_size).padder()
    plaintxt = padder.update(plaintxt)
    plaintxt += padder.finalize()
    print 'after padding:',binascii.hexlify(plaintxt)

    encryptor = cipher.encryptor()
    return encryptor.update(plaintxt) + encryptor.finalize()

password = 'N2I5NDBmN2'
plain = 'a'*16
plain = 'b'*6 + '\x09'
    #aes block size is 16,so need padding
    #when eccrypt update CipherContext will not auto padding,ref
        #https://cryptography.io/en/latest/hazmat/primitives/symmetric-encryption/#cryptography.hazmat.primitives.ciphers.CipherContext
    #so,we need manual padding

# cipher = aesMode(password,'cfb')
cipher = aesMode(password,'cbc')

cipherText = encrypt(cipher,plain)
print 'cipher:',(binascii.hexlify(cipherText))
plain2 = decrypt(cipher,cipherText)
print 'decrypt:',plain2
