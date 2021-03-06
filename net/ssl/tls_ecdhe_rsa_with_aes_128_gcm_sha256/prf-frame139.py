#!/usr/bin/python3
# -*- coding: utf-8 -*-

import hashlib
import hmac
import unittest
import os
import binascii
import sys

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.ciphers import (
    Cipher, algorithms, modes
)

def encrypt(key, plaintext, associated_data,iv):
    # Generate a random 96-bit IV.
    # iv = os.urandom(12)

    # Construct an AES-GCM Cipher object with the given key and a
    # randomly generated IV.
    encryptor = Cipher(
        algorithms.AES(key),
        modes.GCM(iv),
        backend=default_backend()
    ).encryptor()

    # associated_data will be authenticated but not encrypted,
    # it must also be passed in on decryption.
    encryptor.authenticate_additional_data(associated_data)

    # Encrypt the plaintext and get the associated ciphertext.
    # GCM does not require padding.
    ciphertext = encryptor.update(plaintext) + encryptor.finalize()

    return (iv, ciphertext, encryptor.tag)

def decrypt(key, associated_data, iv, ciphertext, tag):
    # Construct a Cipher object, with the key, iv, and additionally the
    # GCM tag used for authenticating the message.
    decryptor = Cipher(
        algorithms.AES(key),
        modes.GCM(iv, tag),
        backend=default_backend()
    ).decryptor()

    # We put associated_data back in or the tag will fail to verify
    # when we finalize the decryptor.
    decryptor.authenticate_additional_data(associated_data)

    # Decryption gets us the authenticated plaintext.
    # If the tag does not match an InvalidTag exception will be raised.
    return decryptor.update(ciphertext) + decryptor.finalize()


def TLSv1_0_PRF(outlen, secret, label, seed):
    ls = len(secret)
    ls1 = ls2 = (ls + 1) // 2

    def xor(xx, yy):
        o = []
        for i in range(len(xx)):
            o.append(xx[i] ^ yy[i])
        return bytes(o)

    md5 = TLSv1_2_PRF(outlen, secret[:ls1], label, seed, hashlib.md5)
    sha1 = TLSv1_2_PRF(outlen, secret[-ls2:], label, seed, hashlib.sha1)

    return xor(md5, sha1)

def TLSv1_2_PRF(outlen, secret, label, seed, h):
    label = bytes(label, 'ASCII')
    secret = bytes(secret)
    seed = bytes(seed)

    def p_hash(hashfn, outlen, k, pt):
        o = []
        a_im = pt
        for i in range(0, outlen, hashfn().digest_size):
            a_i = hmac.new(k, a_im, hashfn).digest()
            output = hmac.new(k, a_i + pt, hashfn).digest()
            o.append(output)
            a_im = a_i
        return bytes(b''.join(o))[:outlen]

    return p_hash(h, outlen, secret, label + seed)

def prfTest():
    out = TLSv1_2_PRF(70,
                bytes('keyforhmac','ASCII'),
                'msg-for-hmac-sha256',
                bytes('','ASCII'),hashlib.sha256)
    print(out.hex())

def test():
    prfTest()
    # sys.exit()

    print('--------prf test ok----------')
    rdClihexStr = 'f77182ed908b500c8b1ad6ad8754329d63ad8704ae8901149727d7257bcf8878'
        #frame 132
    rdSvrhexStr = '59604cc213be22157934682d82a9dbf4cba3f53cc10f6a89d4270bb87a4ebb8c'
        #frame 135
    pre_master_secret_hexStr = '50891929d1f6b3507dfef2416057abb452116d5210c91a2d1c6b2ac4e9df23eeba718ac6b9bd5506479dd99b7585c983'
    pre_master_secret = bytes.fromhex(pre_master_secret_hexStr)
        #from ./firefox-sslkey.log is master_secret

    length = 48
    if len(pre_master_secret) == length:
        #长度是48就认为是master key. firefox sslkeylog 文件中的就是master key
        master_secret = pre_master_secret
    else:
        seedhexStr = rdClihexStr + rdSvrhexStr

        secret = pre_master_secret
        label = "master secret"
        seed = bytes.fromhex(seedhexStr)
        master_secret = TLSv1_2_PRF(length, secret, label, seed, hashlib.sha256)

    print('master: ' + master_secret.hex())

    key_block_secret = master_secret
    seedhexStr = rdSvrhexStr + rdClihexStr

    # TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256
        # sha256      so mac_key_len = 32
        # aes_128_gcm so key_len = 16, iv_len = 16
    maclen = 32
    keylen = 16
    ivlen = 4
    length = (maclen + keylen + ivlen)*2

    """gen key block"""
    secret = key_block_secret
    label = "key expansion"
    seed = bytes.fromhex(seedhexStr)
    key_block = TLSv1_2_PRF(length, secret, label, seed, hashlib.sha256)
    print('keyblock: ' + key_block.hex())
    print('----split-----')

    maclen = 0      #for gcm not need mackey

    # 1
    lenTmp = maclen
    start = 0
    end = start + lenTmp
    client_write_mac_key = bytes(key_block[start:end])
    print('cwmk: '+client_write_mac_key.hex())

    # 2
    start = end
    end = start + lenTmp
    server_write_mac_key = bytes(key_block[start:end])
    print('swmk: '+server_write_mac_key.hex())

    # 3
    lenTmp = keylen
    start = end
    end = start + lenTmp
    client_write_key = bytes(key_block[start:end])
    print(' cwk: '+client_write_key.hex())

    # 4
    start = end
    end = start + lenTmp
    server_write_key = bytes(key_block[start:end])
    print(' swk: '+server_write_key.hex())

    # 5
    lenTmp = ivlen
    start = end
    end = start + lenTmp
    client_write_iv = bytes(key_block[start:end])
    print(' cwi: '+client_write_iv.hex())

    # 6
    start = end
    end = start + lenTmp
    server_write_iv = bytes(key_block[start:end])
    print(' swi: '+server_write_iv.hex())

    plainhexStr = '1400000c4bb5c78b0c01d695180f5ea4'
    plaintext = binascii.unhexlify(plainhexStr)
        #from wireshark after import ./.firefox-sslkey.log

    # frame139
    # Ciphertext[40]:
    # | 00 00 00 00 00 00 00 00 2b 83 0d 98 5e c2 81 6e |........+...^..n|
    # | bd 6d e8 92 bf d3 b4 08 da e0 2d ee a4 aa 98 f1 |.m........-.....|
    # | 8d 48 77 62 fd 72 24 a6                         |.Hwb.r$.        |
    # ssl_decrypt_record: allocating 72 bytes for decrypt data (old len 32)
    # Plaintext[32]:
    # | 14 00 00 0c 4b b5 c7 8b 0c 01 d6 95 18 0f 5e a4 |....K.........^.|
    # | a5 f0 cf 18 da 34 6b 5c f9 4b 0e 6b a2 15 f1 6e |.....4k\.K.k...n|

    # 1603030028                                #record head
    # 0000000000000000                          #explicit nonce
    # 2b830d985ec2816ebd6de892bfd3b408          #cip
    # dae02deea4aa98f18d487762fd7224a6          #tag

    nonceExplicithexStr = '0000000000000000'    #8bytes
    nonceCounter = ''
    nonce = server_write_iv[:4] + binascii.unhexlify(nonceExplicithexStr) + bytes.fromhex(nonceCounter)
    print('non: ' + nonce.hex())

    #ADDlen = 8seq + 1type + 2ver + 2len,
    seq_num = '0000000000000000'            #need be lsb.finished record 为0.
        #8byts,一次tls会话中,key exchange等完成之后，finished record 的seqnum 为0.
    tlsCompressedType = '16'
    tlsCompressedVersion = '0303'
    tlsCompressedLength = '0010'            #没有加密之前的compress length
    associateStr = seq_num +                   \
                    tlsCompressedType +         \
                    tlsCompressedVersion +      \
                    tlsCompressedLength

    associateData = binascii.unhexlify(associateStr)
    print('aso: ' + associateData.hex())


    open('./plaintxt',"bw+").write(plaintext)
    open("./swk","bw+").write(server_write_key)
    open("./swi","bw+").write(server_write_iv)
    open("./associate","bw+").write(associateData)
    open("./nonce","bw+").write(nonce)

    iv, ciphertext, tag = encrypt(
                            server_write_key,
                            plaintext,
                            associateData,
                            nonce
                        )

    print(' iv: ' + iv.hex())
    print('cip: ' + ciphertext.hex())
    print('tag: ' + tag.hex())

    print('-----decrypt-----')
    plaintext = decrypt(
                server_write_key,
                associateData,
                iv,
                ciphertext,
                tag
            )
    print(b'plain: ' + binascii.hexlify(plaintext))

if __name__ == '__main__':
    # unittest.main()
    test()
