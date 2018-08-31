#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
from flask import Flask
from flask import request
from flask import render_template

THIS_FOLDER = os.path.dirname(os.path.abspath(__file__))

app 		= Flask(__name__)
app.debug 	= True

@app.route('/foo', methods=['POST']) 
def foo():
	global THIS_FOLDER

	query = request.form.get('query')
	print("query: ",query)


	MYFILE = 'core/comm/log.txt'
	FILENAME = os.path.join(THIS_FOLDER, MYFILE)
	with open(FILENAME, 'wb') as x_files:
		query_bin = query.encode('utf-8')
		#x_files.write('{}'.format(query))
		x_files.write(query_bin)
		x_files.close()

	APPFILE = 'core/toapp.txt'
	FILENAME = os.path.join(THIS_FOLDER, APPFILE)
	with open(FILENAME, 'r') as r_files:
		content = []
		while (len(content)==0):
			content = r_files.readlines()
		
		r_files.close()
	content = [x.strip() for x in content]

	for ci in range(len(content)):
		cont 		= content[ci]
		cont_list 	= list(cont)
		for i in range(len(cont_list)):
			if (ord(cont_list[i]) == 241):
				cont_list[i] = 'ni'
				#cont_list[i] = 'ñ'
				#print(ord(c))
		cont = "".join(cont_list)
		content[ci] = cont

	for cont in content:
		print cont
	data = {}
	data['titles'] = content
	return render_template('index.html', result=data, ti=content)

@app.route('/')
def home():
	return render_template('index.html')

if __name__ == '__main__':
	app.run(host='0.0.0.0', port=5000, debug=True)