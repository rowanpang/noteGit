#!/usr/bin/python
#coding:utf-8
import cv2

cap = cv2.VideoCapture(0)
if not cap:
    print '摄像头打卡失败!'
    sys.exit(1)
ret, img = cap.read()

cv2.imwrite('pic.jpg', img)
