1,flow
    a, ./chunked.py
    b, curl localhost:9999
    c, firfox localhost:9999
	将会看到下载一个名为'rowanFileName.tmp'的文件. size 192+4=196k

    d,wireshark
	可以看出由2个chunk传递.
