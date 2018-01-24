#!/usr/bin/python
import os
import binascii

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.ciphers import (
    Cipher, algorithms, modes
)

def encrypt(key, iv, plaintext):
    if len(iv) != 16:
        print("error!,for ctr mode len(iv) MUST equal to block size for encrypt,exp 16bytes for gcm");
        return -1

    encryptor = Cipher(
                        algorithms.AES(key),
                        modes.CTR(iv),
                        backend=default_backend()
                    ).encryptor()

    ciphertext = encryptor.update(plaintext) + encryptor.finalize()

    return ciphertext

def decrypt(key, iv, ciphertext):
    decryptor = Cipher(
                        algorithms.AES(key),
                        modes.CTR(iv),
                        backend=default_backend()
                    ).decryptor()
    plaintxt = decryptor.update(ciphertext) + decryptor.finalize()

    return plaintxt

swkhexStr = '5e51e695f1dfd68e362c1db2400342e0'
swihexStr = 'e2c3e5c0'

server_write_key = binascii.unhexlify(swkhexStr)
server_write_iv = binascii.unhexlify(swihexStr)

#record from ~/noteGit/net/ssl/tls_ecdhe_rsa_with_aes_128_gcm_sha256/TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256.pcapng
sslRecordStr = '160303002800000000000000002b830d985ec2816ebd6de892bfd3b408dae02deea4aa98f18d487762fd7224a6'                 #frame 139
sslRecordStr = '170303007800000000000000028c1d55b765f93776bbdcea3ba0338b3f403347cb1f4f5ca6e4362a1c08ff82630821dba79993d2532244237df91536098fe71ed67dce51ea436879fa14bd66980854bda942824505b527baa12c6f5fef33899edfbed8decaf727829dfe81a705eeee3c422fb63a592f1bb60ba9c78d53'                                 #frme 169

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
print('nce: ' + binascii.hexlify(nonce))

cipherOffset = nonceEnd
ciphertext = sslRecord[cipherOffset:]
print('str: ' + binascii.hexlify(ciphertext))

print('-----decrypt-----')
plaintext = decrypt(
                server_write_key,
                nonce,
                ciphertext
            )
taglen = 16
trueLen = len(plaintext) - taglen

print('plt: ' + binascii.hexlify(plaintext[:trueLen]))
