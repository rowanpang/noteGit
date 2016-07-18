#!/usr/bin/python
import os
import time
import socket
def __servicelist():
    """
    Get service information
    """
    result = []  # used to return
    #print(os.popen('chkconfig --list'))
    services = os.popen('chkconfig --list').readlines()
    output=open("./chk_info.txt","w+")
    output.write("-------------"+time.strftime("%a, %d %b %Y %H:%M:%S +0000")+"--------\n")

    sonsk=socket.socket(socket.AF_UNIX,socket.SOCK_STREAM)
    sonsk.connect("./sk_file")
    sonsk.send("son recv-------")

    #print services 
    for oneline in services:
        line = oneline.split()
        if len(line) <6:
            continue
        output.write(oneline)
        service = {
            'name': line[0],  # service name
            'L0': line[1][2:],  # level0
#            'L1': line[2][2:],  # level1
#            'L2': line[3][2:],  # level2
#            'L3': line[4][2:],  # level3
#            'L4': line[5][2:],  # level4
#            'L5': line[6][2:],  # level5
#            'L6': line[7][2:],  # level6
        }
        result.append(service)
    output.flush()
    output.close()
    return result

if __name__ == '__main__':
    print("hello world,from son.py")
    __servicelist()
    time.sleep(100);
