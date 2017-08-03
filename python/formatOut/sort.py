#!/usr/bin/python

def parserLine(line,idxMap,valLenMap):
    buf=[]
    dicbuf={}
    if not line.strip():
        return
    else:
        values = line.split(",")
        for k in golKeys:
            if idxMap[k] != None:
                buf.append(values[idxMap[k]])
                dicbuf[k] = values[idxMap[k]]
                if valLenMap[k] < len(values[idxMap[k]]):
                    valLenMap[k] = len(values[idxMap[k]])

        # print buf
        # print dicbuf
    return buf,dicbuf

def parserKey(line = None):
    keyIndexMap = {}.fromkeys(golKeys)
    strbuf=""
    if not line.strip():
        return
    else:
        keys = line.split(",")
        index = 0;
        for k in keys:
            for ktmp in keyIndexMap.keys():
                if k.strip() == ktmp.strip():
                    # print "match!!! %s %d" %(ktmp,index)
                    keyIndexMap[ktmp] = index;
                    break;
            index += 1

    return keyIndexMap

def formatOut(dicts,lists,valLenMap,idxMap):
    header=""
    valFmt = {}.fromkeys(golKeys)
    for k in golKeys:
        valFmt[k] = '%%-%ds   ' %(valLenMap[k])
        if idxMap[k] != None:
            header += k.center(valLenMap[k]) + " "*3
    print header

    func = lambda s:s[sortBy]
    dicts.sort(key=func)
    for val in dicts:
        strbuf=""
        for k in golKeys:
            if idxMap[k] != None:
                strbuf += valFmt[k] %(val[k])
        print strbuf

    # for val in lists:
        # i = 0
        # strbuf=""
        # for k in golKeys:
            # strbuf += valFmt[k] %(val[i])
            # i += 1
        # print strbuf
    return

def main():
    f = open("./wep-phomt-01.csv")
    listListbuf=[]
    dicListbuf=[]

    lenMap = {}.fromkeys(golKeys,0)
    for k in golKeys:
        lenMap[k] = len(k)
    # print lenMap

    while True:
        line = f.readline()
        if not line:
            # print "reach EOF"
            break;
        elif not line.strip() or line.strip().startswith("#"):  
            # print "ignore space line and line start with #"
            continue
        elif line.startswith("BSSID,"):
            keyIdxMap = parserKey(line)
        elif line.startswith("Station MAC,"):
            break
        else:
            vallist,valdic = parserLine(line,keyIdxMap,lenMap)
            listListbuf.append(vallist)
            dicListbuf.append(valdic)

    formatOut(dicListbuf,listListbuf,lenMap,keyIdxMap)
    f.close()

sortBy='channel'
golKeys = [
            "BSSID",
            "channel",
            "Speed",
            "Privacy",
            "ignoreKey",            #will ignore it for did not exist
            "Cipher",
            "Authentication",
            "Power",
            "ESSID"]

main()
