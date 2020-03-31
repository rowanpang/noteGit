1,basic
    a,arg0,在ps 中看到的 name
	完全由execev的argv参数决定.

2,log
    $ ./a.out

    20 bytes,0xa:0:0		#fir read
    hello world,sleep 3

    16 bytes,0xa:0:0		#sed read
    sleep 2 seconds

    0 bytes,0:0:0		#child exit read
    child exit 0
    father exit
