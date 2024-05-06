#!/usr/bin/python3
import os
import binascii

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

#from frame139
# 1603030028                                #record head
# 0000000000000000                          #explicit nonce
# 2b830d985ec2816ebd6de892bfd3b408          #cip
# dae02deea4aa98f18d487762fd7224a6          #tag

#1 swk/swi
swkhexStr = '5e51e695f1dfd68e362c1db2400342e0'
swihexStr = 'e2c3e5c0'
server_write_key = binascii.unhexlify(swkhexStr)
server_write_iv = binascii.unhexlify(swihexStr)

cipherHexStr = '2b830d985ec2816ebd6de892bfd3b408'
cipherHex = binascii.unhexlify(cipherHexStr)

#2 associated data
seq_num = '0000000000000000'
tlsCompressedType = '16'
tlsCompressedVersion = '0303'
tlsCompressedLength = '%04x' %(len(cipherHex))
additionalStr = seq_num +                   \
                tlsCompressedType +         \
                tlsCompressedVersion +      \
                tlsCompressedLength
additionalData = binascii.unhexlify(additionalStr)

#3 iv,nonce
nonceExplicithexStr = '6de892bfd3b408dae0'
nonceExplicithexStr = '0000000000000000'
nonce = server_write_iv[:4] + binascii.unhexlify(nonceExplicithexStr)
iv = nonce

#4 ciphertext
ciphertext = cipherHex

#5 tag
taghexStr = 'dae02deea4aa98f18d487762fd7224a6'
tag = binascii.unhexlify(taghexStr)

print('------------ssl frame139 record info---------------------')
print('noncelen:', len(nonce))
print("iv/nce:", binascii.hexlify(nonce))
print("   add:", additionalStr)
print("cipper:", cipherHexStr)
print("   tag:", taghexStr)


print()
print('------------decrypt---------------------')
plaintext = decrypt(
            server_write_key,
            additionalData,
            iv,
            ciphertext,
            tag
        )
print('plain:', binascii.hexlify(plaintext))
print('ptLen:', len(plaintext))

print()
print('------------encrypt---------------------')

plainhexStr = '1400000c4bb5c78b0c01d695180f5ea4'
plaintext = binascii.unhexlify(plainhexStr)

iv, ciphertext, tag = encrypt(
                        server_write_key,
                        plaintext,
                        additionalData,
                        nonce
                    )
print(" iv: ", binascii.hexlify(iv))
print("cip: ", binascii.hexlify(ciphertext))
print("tag: ", binascii.hexlify(tag))
