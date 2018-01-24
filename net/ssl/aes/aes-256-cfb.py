#!/usr/bin/python
import os
import binascii
import hashlib

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.ciphers import (
    Cipher, algorithms, modes,BlockCipherAlgorithm
)

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

password = 'N2I5NDBmN2'

def aes256cfb(pwd):
    key,iv = EVP_BytesToKey(pwd,32,16)

    algorithm = algorithms.AES(key)
    blockSize = algorithm.block_size

    mode = modes.CFB(iv)
    return Cipher(algorithm, mode, backend=default_backend())

def decrypt(cipher,cipherHex):
    decryptor = cipher.decryptor()
    return decryptor.update(cipherHex) + decryptor.finalize()

def encrypt(cipher,plaintxt):
    encryptor = cipher.encryptor()
    return encryptor.update(plaintxt) + encryptor.finalize()

print
print
plain = 'aaaaaasaa'
cipherText = encrypt(aes256cfb(password),plain)
print(binascii.hexlify(cipherText))
plain2 = encrypt(aes256cfb(password),cipherText)
print plain2
