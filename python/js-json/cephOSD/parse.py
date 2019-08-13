#!/usr/bin/python

import os
import sys
import json

# os.system('icfs osd tree -f json-pretty > osddf.json')
# os.system('icfs osd df -f json-pretty > osddf.json')

fposdtree=open('./osdtree.json','r')
osdtree=json.load(fposdtree)

nodes=osdtree["nodes"]

hosts=[]

for node in nodes:
	nodetype=node["type"]
	if nodetype == "host":
		name=node["name"]
		children=node["children"]
		if len(children) >= 2:
			#print "hint host",name
			#print children

			hostexist=0
			for h in hosts:
				if h["name"]==name:
					hostexist=1;
					break;
			if hostexist == 0:
				tmp={}
				tmp["name"]=name
				#print type(children)
				children.sort()
				tmp["children"]=children
				hosts.append(tmp)

#print hosts


fposddf=open('./osddf.json','r')
osddf=json.load(fposddf)
dfnodes=osddf["nodes"]

dfstates=[]
for h in hosts:
	name=h["name"]
	children=h["children"]
	df={}
	df["name"]=name
	kbTotal=0
	every={}
	for osd in children:
		osdname="osd.%d" %(osd)
		#print name,osdname
		for dfnode in dfnodes:
			if dfnode["type"]=="osd" and dfnode["name"]==osdname:
				if osdname not in every.keys():
					every[osdname]=dfnode["kb_used"]
					kbTotal=kbTotal+dfnode["kb_used"]
	df["every"]=every
	df["total"]=kbTotal

	print "%s:total-kb_used:%d" %(name,kbTotal)

	dfstates.append(df)

#print dfstates

#print json.dumps(dfstates,indent=4)
