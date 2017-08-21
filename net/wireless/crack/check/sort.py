#!/usr/bin/python
import sys
import getopt
import os

def parserLine(line,idxMap,valLenMap,keys):
    dicbuf={}
    if not line.strip():
        return
    else:
        values = line.split(",")
        for k in keys:
            if idxMap[k] != None:
                dicbuf[k] = values[idxMap[k]]
                if valLenMap[k] < len(values[idxMap[k]]):
                    valLenMap[k] = len(values[idxMap[k]])

        # print buf
        # print dicbuf
    return dicbuf

def parserKey(line,keys):
    fmtLenMap = {}.fromkeys(keys,0)
    keyIndexMap = {}.fromkeys(keys)

    for k in keys:
        fmtLenMap[k] = len(k)

    strbuf=""
    if not line.strip():
        return
    else:
        vkeys = line.split(",")
        index = 0;
        for k in vkeys:
            for ktmp in keyIndexMap.keys():
                if k.strip() == ktmp.strip():
                    # print "match!!! %s %d" %(ktmp,index)
                    keyIndexMap[ktmp] = index;
                    break;
            index += 1

    return fmtLenMap,keyIndexMap

def formatOut(dicts,valLenMap,idxMap,keys,sortBy,keysfilter=None):
    header=""
    valFmt = {}.fromkeys(keys)
    for k in keys:
        valFmt[k] = '%%-%ds   ' %(valLenMap[k])
        if idxMap[k] != None:
            header += k.center(valLenMap[k]) + " "*3
    print header

    func = lambda s:s[sortBy]
    dicts.sort(key=func)
    for val in dicts:
        strbuf=""
        fv = None
        for k in keys:
            if keysfilter and k == keysfilter['key']:
                fv = val[k]
            if idxMap[k] != None:
                strbuf += valFmt[k] %(val[k])
        if keysfilter:
            # print fv
            # print keysfilter['val']
            if fv.strip() == keysfilter['val'].strip():
                 print strbuf
        else:
            print strbuf

    return

def usage():
    i=0
    print 'Usage:'
    print '%5d),%s -e "essid" [x.cvs]' %(i,os.path.basename(sys.argv[0]))
    i+=1
    print '%5d),%s -s "sortBy" %s [x.cvs]' %(i,os.path.basename(sys.argv[0]),apKeys)
    i+=1
    print '%5d),%s [x.cvs]' %(i,os.path.basename(sys.argv[0]))

def argsParser():
    global pickOut,tarESSID,apSorted
    # usage()
    opts, args = getopt.getopt(sys.argv[1:], "hs:e:")
    for op, value in opts:
	if op == "-e":
            pickOut = True 
	    tarESSID = value
        elif op == '-s':
            apSorted = value
	elif op == "-h":
            usage()
            sys.exit()

def main():

    argsParser()

    f = open("./radioCheck-01.csv")
    apInfos=[]
    stInfos=[]

    parserAPClient = False 
    while True:
        line = f.readline()
        if not line:
            if pickOut:
                stfilter = {}
                stfilter['key'] = 'BSSID'
                stfilter['val'] = tarBSSID
                print stfilter
                formatOut(stInfos,fmtLenMap,keyIdxMap,stKeys,stSorted,stfilter)
            else:
                formatOut(stInfos,fmtLenMap,keyIdxMap,stKeys,stSorted)
            break;
        elif not line.strip() or line.strip().startswith("#"):  
            # print "ignore space line and line start with #"
            continue
        elif line.startswith(lineIndicatorAP):
            fmtLenMap,keyIdxMap = parserKey(line,apKeys)
        elif line.startswith(lineIndicatorST):
            formatOut(apInfos,fmtLenMap,keyIdxMap,apKeys,apSorted)
            fmtLenMap,keyIdxMap = parserKey(line,stKeys)
            parserAPClient = True
        else:
            if not parserAPClient:
                valdic = parserLine(line,keyIdxMap,fmtLenMap,apKeys)
                apInfos.append(valdic)                               #dict in list buf
                if pickOut and valdic["ESSID"].strip() == tarESSID:
                    print 'channel:' + valdic["channel"].strip() + ',  bssid:' + valdic["BSSID"]
                    tarBSSID = valdic["BSSID"]
            else:
                valdic = parserLine(line,keyIdxMap,fmtLenMap,stKeys)
                stInfos.append(valdic)                               #dict in list buf

    f.close()

lineIndicatorAP = 'BSSID,'
lineIndicatorST = 'Station MAC,'

stKeys = [
            "Station MAC",
            "Power",
            "BSSID"]
stSorted='BSSID'

apKeys = [
            "BSSID",
            "channel",
            "Speed",
            "Privacy",
            "ignoreKey",            #will ignore it for did not exist
            "Cipher",
            "Authentication",
            "Power",
            "ESSID"]

apSorted='Power'
tarESSID = ""
tarBSSID = ""
pickOut = False

main()
