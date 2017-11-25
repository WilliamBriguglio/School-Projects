from Cryptodome.Cipher import AES
import sys
import timeit
import os
from decimal import Decimal
from Cryptodome.PublicKey import RSA
from Cryptodome.Cipher import PKCS1_OAEP

#____________________________________FUNCTIONS____________________________________

def readKey(RSAKeyFile, AESKeyFile):
    encoded_key = open(RSAKeyFile, "rb").read() #retrieve RSA private key
    RSA_key = RSA.import_key(encoded_key)       #decode
    cipher_RSA = PKCS1_OAEP.new(RSA_key)        #create RSA cipher object
    with open(AESKeyFile, "rb") as f:
        AES_key = cipher_RSA.decrypt(f.read())
    return AES_key

def readIV(RSAKeyFile, IVfile):
    encoded_key = open(RSAKeyFile, "rb").read()  # retrieve RSA private key
    RSA_key = RSA.import_key(encoded_key)  # decode
    cipher_RSA = PKCS1_OAEP.new(RSA_key)  # create RSA cipher object
    with open(IVfile, "rb") as f:
        IV = cipher_RSA.decrypt(f.read())
    return IV

#____________________________________CODE____________________________________

tic = timeit.default_timer()

#check arguments
if( len(sys.argv) != 6):
    print("USAGE: encrypt.py [RSA_key] [AES_key.txt] [IV.txt] [plain.txt] [cipher.txt]")
    sys.exit()

#print file size
statinfo = os.stat(sys.argv[4])
ptsize = statinfo.st_size
print("Size of file in bytes: ", ptsize)

#open plaint text and read in lines
chunkSize = 144
with open(sys.argv[4], "rb") as plainfp:

    #create cipher object
    cipher = AES.new(readKey(sys.argv[1], sys.argv[2]), AES.MODE_OFB, iv=readIV(sys.argv[1], sys.argv[3]))

    #create cipher text file
    with open(sys.argv[5], "wb") as cipherfp:

        #encrypt and write
        amountprocessed = 0;
        chunk = b'0'
        while (chunk != b''):
            chunk = plainfp.read(chunkSize)
            ct = cipher.encrypt(chunk)
            cipherfp.write(ct)

            # print percent of plaintext processed
            amountprocessed = Decimal(amountprocessed) + Decimal(len(chunk))
            percentprocessed = (amountprocessed / ptsize) * 100
            print(percentprocessed.quantize(Decimal('0.01')), end="%\r")

#display time
toc = timeit.default_timer()
elapsed = toc - tic
unit = "sec"
if(elapsed > 60):
    elapsed = elapsed/60
    unit = "mins"
print("\nTime Elapsed: ", elapsed, unit)