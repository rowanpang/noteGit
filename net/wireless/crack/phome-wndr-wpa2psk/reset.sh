#!/bin/bash
airmon-ng stop wlp2s0mon
nmcli radio wifi off
sleep 0.5
nmcli radio wifi on
