#!/usr/bin/python
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

key='t' * 16
swkhexStr = '386bb729928e9471a590e87490c29c36'
swihexStr = '6a12a0193df575107859644dda80478c'

server_write_key = binascii.unhexlify(swkhexStr)
server_write_iv = binascii.unhexlify(swihexStr)

# 1603030028
# 00000000000000002b830d985ec2816ebd
# 6de892bfd3b408dae0
# 2deea4aa98f18d487762fd7224a6
nonceExplicithexStr = '6de892bfd3b408dae0'
nonce = server_write_iv[:4] + binascii.unhexlify(nonceExplicithexStr)
print('nce: ' + binascii.hexlify(nonce))

seq_num = 'f7360d5c'
tlsCompressedType = '16'
tlsCompressedVersion = '0303'
tlsCompressedLength = '0028'
additionalStr = seq_num +                   \
                tlsCompressedType +         \
                tlsCompressedVersion +      \
                tlsCompressedLength         
additionalData = binascii.unhexlify(additionalStr)
plainhexStr = '1400000c4bb5c78b0c01d695180f5ea4'
plaintext = binascii.unhexlify(plainhexStr)

iv, ciphertext, tag = encrypt(
                        server_write_key,
                        plaintext,
                        additionalData,
                        nonce
                    )

print('tag: ' + binascii.hexlify(tag))
print(' iv: ' + binascii.hexlify(iv))
print('cip: ' + binascii.hexlify(ciphertext))

print('-----decrypt-----')
plaintext = decrypt(
            server_write_key,
            additionalData,
            iv,
            ciphertext,
            tag
        )
print binascii.hexlify(plaintext)
