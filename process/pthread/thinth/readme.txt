1,base
    a,cancel 默认是异步的,延期执行. ref man pthread_cancel.
    b,即便cancel是在disable阶段发送的,当enable时依然有效.

2,output log parser
    in main:21081
    in thread:21082
    start with disable canceled
    sig cancele was emitted
	----->here will be a wait. for sleep(5) in thread func.
	----->after enable cancel, cancel it.
    start enable canceled
    thread was canceled

