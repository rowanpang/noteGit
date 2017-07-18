#!/usr/bin/python
#coding:utf-8
from flask import Flask
from SystemResource.main import SystemResourceRoute

app = Flask(__name__)
app.register_blueprint(SystemResourceRoute, url_prefix='/SystemResource')
app.run(host = '0.0.0.0', port=8080)

# 文档 http://flask.pocoo.org/docs/0.10/blueprints/
