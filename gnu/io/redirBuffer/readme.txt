1,basic
    a,main 执行2次
	./main			#stdio 为终端 文件. 'linux 一切皆文件'.
	./main | tee /dev/null	#stdio 为pipe 文件.

    b,针对 全缓冲,行缓冲,无缓冲 解释.
	1),这是fopen(xx),等libc api提供的机制.
	2),对于unistd的write() .. 等posix系统调用不适用(这些没有缓存机制.)

2,./main
    a,结果:
	------write with \n---
	------write 1------------wr------write 2-------------p-------print \n----
	--------fwrite stdout \n------
	--------fwrite stderr 1----------------------fwrite stderr \n------
	;4
	    ******************rowan 注释, 等待结束输出*************
	-------p-------print ------------fwrite stdout 1--------------fw

    b,分析:
	1),write(x) 及 write(x\n) 均立即输出,说明 write()调用不存在缓冲机制,直接输出.
	2),print(x) 及 print(x\n)
	    print 是libc 提供的api,与fopen(xx)等一个级别.
	    默认使用FILE* stdout
	    (x)没有立即输出，而是等待程序结束时刷新出来的.
	    (x\n),直接输出,行缓存输出.
	3),fwrite(x,stdout),fwrite(\n,stdout)
	    与print()分析类似.
	4),fwrite(x,stderr),fwrite(\n,stderr)
	    2个都会直接输出,  不存在缓存.

3,./main | tee /dev/null
    a,结果
	--------fwrite stderr 1----------------------fwrite stderr \n------
	;4------write with \n---
	------------write 1--------------wr------write 2---------------p
	    ******************rowan 注释, 等待结束输出*************
	-------print \n----
	--------fwrite stdout \n------
	-------p-------print ------------fwrite stdout 1--------------fw

    b,分析:
	1),立即输出的只有write(x, x\n) fwrite(stderr x, x\n)
	2),对于非终端stdout 都会缓冲输出,全缓冲.
