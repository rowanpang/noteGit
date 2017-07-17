#!/usr/bin/python

import os
from flask import Flask,jsonify
import json

app = Flask(__name__)

@app.route('/')
def index():
    return '-from --- hello --- flask-'

@app.route('/jsonify')
def jsonifyTest():
    ret={}
    ret["a"] = "aaaaaaa"
    ret['b'] = 'bbbbbbb'
    ret['c'] =  3333333
    return jsonify(ret)

@app.route('/jsondump')
def jsondump():
    ret0={}
    ret1={}
    ret2={}
    l = []

    ret0["a"] = "Aaaaaaaa"
    ret0['b'] = 'Abbbbbbb'
    ret0['c'] =  13333333
    l.append(ret0)

    ret1["a"] = "Baaaaaaa"
    ret1['b'] = 'Bbbbbbbb'
    ret1['c'] =  23333333
    l.append(ret0)

    print l

    ret2['num'] = 2
    ret2['info'] = l
    return json.dumps(ret2)

if "__main__" == __name__:
    app.run(host='127.0.0.1',port=5000,debug = True)
