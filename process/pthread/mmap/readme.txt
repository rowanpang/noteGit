1,mmap 多线程锁竞争问题
2,./a.out 3 1000000
    top , 查看sys利用率.

3,perf
    $ perf record -g  ./a.out 3 1000000
	in main:1166
	pthread joining
	thread: 1, iters: 1000000
	thread: 0, iters: 1000000
	thread: 2, iters: 1000000
	thread: 0, exit
	pthread 0,finished,elapsed usec :3511615
	thread: 0, exit
	pthread 1,finished,elapsed usec :3524687
	thread: 0, exit
	pthread 2,finished,elapsed usec :3525601
	[ perf record: Woken up 13 times to write data ]
	[ perf record: Captured and wrote 3.437 MB perf.data (27079 samples) ]

    $ perf report -i perf.data
