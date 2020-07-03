

1,debugfs 目录结构
    #define TRACE_SYSTEM xhr_test

    /*
     *$ ll /sys/kernel/debug/tracing/events/xhr_test/
     *-rw-r--r-- 1 root root 0 Jul  3 23:19 enable
     *-rw-r--r-- 1 root root 0 Jul  3 23:19 filter
     *drwxr-xr-x 2 root root 0 Jul  3 23:19 te_test
     */

    TRACE_EVENT(te_test,
	TP_PROTO(int num),
	TP_ARGS(num),
	TP_STRUCT__entry(
	    __field(int, output)
	    __field(int, count)
	),
	TP_fast_assign(
	    __entry->count++;
	    __entry->output = num;
	),
	TP_printk("count=%d output=%d", __entry->count, __entry->output)
    );

2,test
    a, insmod xhr.ko
	cd /sys/kernel/debug/tracing/
	echo 1 > tracing_on
	echo 1 > events/xhr_test/te_test/enable

    b, $ cat trace
	# tracer: nop
	#
	# entries-in-buffer/entries-written: 15/15   #P:4
	#
	#                              _-----=> irqs-off
	#                             / _----=> need-resched
	#                            | / _---=> hardirq/softirq
	#                            || / _--=> preempt-depth
	#                            ||| /     delay
	#           TASK-PID   CPU#  ||||    TIMESTAMP  FUNCTION
	#              | |       |   ||||       |         |
	xhr-thread-17811 [003] .... 4029511.518473: te_test: count=1 output=409
	xhr-thread-17811 [003] .... 4029512.542522: te_test: count=74764011 output=410
	xhr-thread-17811 [003] .... 4029513.566575: te_test: count=-317757560 output=411
	xhr-thread-17811 [003] .... 4029514.590622: te_test: count=1 output=412
	xhr-thread-17811 [003] .... 4029515.614684: te_test: count=1 output=413
	xhr-thread-17811 [003] .... 4029516.638743: te_test: count=1 output=414
	xhr-thread-17811 [003] .... 4029517.662795: te_test: count=9591 output=415
	xhr-thread-17811 [003] .... 4029518.686850: te_test: count=833 output=416
	xhr-thread-17811 [003] .... 4029519.710880: te_test: count=4 output=417
	xhr-thread-17811 [003] .... 4029520.734973: te_test: count=613 output=418
	xhr-thread-17811 [003] .... 4029521.759026: te_test: count=32766 output=419
	xhr-thread-17811 [003] .... 4029522.783069: te_test: count=51 output=420
	xhr-thread-17811 [003] .... 4029523.807124: te_test: count=587883 output=421
	xhr-thread-17811 [003] .... 4029524.831166: te_test: count=-1341826007 output=422
	xhr-thread-17811 [003] .... 4029525.855236: te_test: count=1 output=423
