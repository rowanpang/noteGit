#!/bin/bash
#Step 6 - Run aircrack-ng to obtain the WEP key
    aircrack-ng -w ./pwd.lst -b 6C:B0:CE:17:AD:71 ./wpa2psk*.cap
