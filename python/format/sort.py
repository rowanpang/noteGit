#!/usr/bin/python

def parserLine(line,idxMap,valLenMap):
    buf=[]
    if not line.strip():
        return
    else:
        values = line.split(",")
        for k in golKeys:
            if idxMap[k] != None:
                buf.append(values[idxMap[k]])
                if valLenMap[k] < len(values[idxMap[k]]):
                    valLenMap[k] = len(values[idxMap[k]])

        # print buf
        # print strbuf
    return buf

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

def formatOut(valbuf,valLenMap,idxMap):
    header=""
    valFmt = {}.fromkeys(golKeys)
    for k in golKeys:
        valFmt[k] = '%%-%ds ' %(valLenMap[k])
        header += k.center(valLenMap[k]) + " "
    # print valFmt
    print header

    for val in valbuf:
        i = 0
        strbuf=""
        for k in golKeys:
            strbuf += valFmt[k] %(val[i])
            i += 1
        print strbuf

def main():
    f = open("./wep-phomt-01.csv")
    valbuf=[]

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
            valbuf.append(parserLine(line,keyIdxMap,lenMap))

    formatOut(valbuf,lenMap,keyIdxMap)
    f.close()

golKeys = [
            "BSSID",
            "channel",
            "Speed",
            "Privacy",
            "Cipher",
            "Authentication",
            "Power",
            "ESSID"]

main()
