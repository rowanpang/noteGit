#!/usr/bin/python

def parserLine(line,keymap):
    buf=[]
    strbuf=""
    if not line.strip():
        return
    else:
        # print line,
        values = line.split(",")
        # print values
        for k in golKeys:
            if keymap[k] != None:
                # print k
                # print keymap[k]
                # print values[keymap[k]]
                buf.append(values[keymap[k]])
                strbuf += golKeyValFmt[k] %(values[keymap[k]])

        # print buf
        print strbuf

def parserKey(line = None):
    keyIndexMap = {}.fromkeys(golKeys)

    if not line.strip():
        return
    else:
        print line,
        keys = line.split(",")
        index = 0;
        for k in keys:
            # print "------------"
            # print "key from line: %s" %(k)
            for ktmp in keyIndexMap.keys():
                # print ktmp
                if k.strip() == ktmp.strip():
                    # print "match!!! %s %d" %(ktmp,index)
                    keyIndexMap[ktmp] = index;
                    break;
            index += 1

    print keyIndexMap
    return keyIndexMap

def main():
    f = open("./wep-phomt-01.csv")
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
            parserLine(line,keyIdxMap)

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

golKeyValFmt = {
        "BSSID":'%-18s\t',
        "channel":'%-3s\t',
        "Speed":'%-3s\t',
        "Privacy":"%-10s\t",
        "Cipher":'%-10s\t',
        "Authentication":'-%4s\t',
        "Power":'%-4s\t',
        "ESSID":'%-s\t' }

main()
