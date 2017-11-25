import os
from Cryptodome.PublicKey import RSA
from Cryptodome.Cipher import PKCS1_OAEP

RSA_PrivateKey = RSA.generate(2048)       #generate RSA keys
RSA_PublicKey = RSA_PrivateKey.publickey()

with open('RSAPrivateKey.bin','wb') as f:      #save private key
    f.write(RSA_PrivateKey.exportKey())
    f.close()

with open('RSAPublicKey.bin','wb') as f:      #save public key
    f.write(RSA_PublicKey.exportKey())
    f.close()

AES_key = os.urandom(32)                #generate AES key
AES_IV = os.urandom(16)                 #generate initializaion vector

cipher_RSA = PKCS1_OAEP.new(RSA_PublicKey)    #create cipher object

with open("AESkey.txt", "wb") as f:
    f.write(cipher_RSA.encrypt(AES_key))#encrypt save AES key

with open("IV.txt", "wb") as f:
    f.write(cipher_RSA.encrypt(AES_IV))#encrypt and save initialization vector