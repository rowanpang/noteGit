#!/usr/bin/python
import os
import binascii

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.ciphers import (
    Cipher, algorithms, modes
)

def encrypt(key, iv, plaintext):
    if len(iv) != 16:
        print("error!,for ctr mode len(iv) MUST equal to block size,exp 16bytes for aes");
        return -1

    encryptor = Cipher(
                        algorithms.AES(key),
                        modes.CTR(iv),
                        backend=default_backend()
                    ).encryptor()

    ciphertext = encryptor.update(plaintext) + encryptor.finalize()

    return ciphertext

def decrypt(key, iv, ciphertext):

    cipher = Cipher( algorithms.AES(key), modes.CTR(iv), backend=default_backend())
    cipher.mode.validate_for_algorithm(cipher.algorithm)
        #if invalid will raise error.
    print cipher.mode.name

    decryptor = cipher.decryptor()

    plaintxt = decryptor.update(ciphertext) + decryptor.finalize()

    return plaintxt

swkhexStr = '5e51e695f1dfd68e362c1db2400342e0'
swihexStr = 'e2c3e5c0'

server_write_key = binascii.unhexlify(swkhexStr)
server_write_iv = binascii.unhexlify(swihexStr)

#record from ~/noteGit/net/ssl/tls_ecdhe_rsa_with_aes_128_gcm_sha256/TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256.pcapng
sslRecordStr = '160303002800000000000000002b830d985ec2816ebd6de892bfd3b408dae02deea4aa98f18d487762fd7224a6'                 #frame 139
sslRecordStr = '170303007800000000000000028c1d55b765f93776bbdcea3ba0338b3f403347cb1f4f5ca6e4362a1c08ff82630821dba79993d2532244237df91536098fe71ed67dce51ea436879fa14bd66980854bda942824505b527baa12c6f5fef33899edfbed8decaf727829dfe81a705eeee3c422fb63a592f1bb60ba9c78d53'                                 #frme 169
    # Ciphertext[120]:
    # | 00 00 00 00 00 00 00 02 8c 1d 55 b7 65 f9 37 76 |..........U.e.7v|
    # | bb dc ea 3b a0 33 8b 3f 40 33 47 cb 1f 4f 5c a6 |...;.3.?@3G..O\.|
    # | e4 36 2a 1c 08 ff 82 63 08 21 db a7 99 93 d2 53 |.6*....c.!.....S|
    # | 22 44 23 7d f9 15 36 09 8f e7 1e d6 7d ce 51 ea |"D#}..6.....}.Q.|
    # | 43 68 79 fa 14 bd 66 98 08 54 bd a9 42 82 45 05 |Chy...f..T..B.E.|
    # | b5 27 ba a1 2c 6f 5f ef 33 89 9e df be d8 de ca |.'..,o_.3.......|
    # | f7 27 82 9d fe 81 a7 05 ee ee 3c 42 2f b6 3a 59 |.'........<B/.:Y|
    # | 2f 1b b6 0b a9 c7 8d 53                         |/......S        |
    # Plaintext[112]:   txt + 16tag.
    # | a4 cf 6e d8 c8 c9 3f 28 f8 6d e9 09 0b ac 64 d8 |..n...?(.m....d.|
    # | 58 55 30 0e 80 c8 11 f6 96 be 1e 0f e1 2c 15 f7 |XU0..........,..|
    # | 6c 9d 63 08 3e 74 4d 46 11 aa aa 72 91 f2 8b 9b |l.c.>tMF...r....|
    # | f3 a2 eb b8 ae 54 ec 5f 85 be 4f 0d 02 b0 d5 73 |.....T._..O....s|
    # | 59 b4 b2 ac 84 23 25 2a 90 8d 44 c9 0a 09 7b 4c |Y....#%*..D...{L|
    # | 4a 79 32 0d 4a bf c5 79 9c a8 7a 94 6e 9c 66 5c |Jy2.J..y..z.n.f\|
    # | 2c 06 fc 98 ea 5e 2c 21 c9 75 c9 61 33 5a a9 d6 |,....^,!.u.a3Z..|

sslRecord = binascii.unhexlify(sslRecordStr)
start = 0;
tmplen = 1
end = start + tmplen
conType = sslRecord[start:end]      #content type

start = end
tmplen = 2
end = start + tmplen
sslVer = sslRecord[start:end]

start = end
tmplen = 2
end = start + tmplen
conLen = sslRecord[start:end]
conOffset = end

nonceOffset = conOffset
nonceExplicithexLen = 8
nonceEnd = nonceOffset + nonceExplicithexLen
nonceExplicithex = sslRecord[conOffset:nonceEnd]
ctrStr = '00000002'
    #why use 2, ref wireshark-2.1.1/epan/dissectors/packet-ssl-utils.c :3534
    # /* NIST SP 800-38D, sect. 7.2 says that the 32-bit counter part starts
    # * at 1, and gets incremented before passing to the block cipher. */
nonce = server_write_iv[:4] + nonceExplicithex + binascii.unhexlify(ctrStr)
    #nonce len = 4+8+4 = 16bytes
print('nce: ' + binascii.hexlify(nonce))

cipherOffset = nonceEnd
ciphertext = sslRecord[cipherOffset:]
print('cipher str: ' + binascii.hexlify(ciphertext))

print
print('-----use ctr mode decrypt tls_ecdhe_rsa_with_aes_128_gcm_sha256,frame 169-----')
print
plaintext = decrypt(
                server_write_key,
                nonce,
                ciphertext
            )
taglen = 16
trueLen = len(plaintext) - taglen

print('plt: ' + binascii.hexlify(plaintext[:trueLen]))
