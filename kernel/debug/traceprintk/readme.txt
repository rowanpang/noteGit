1,about file name
    a,不要使用ftrace.c,在生成_mcount_loc section时会跳过这个命名的文件.
    b,ref ./scripts/recordmcount.c
	const char ftrace[] = "/ftrace.o";
	int ftrace_size = sizeof(ftrace) - 1;

	char *file = argv[i];
	len = strlen(file);
	if (len >= ftrace_size && strcmp(file + (len - ftrace_size), ftrace) == 0)
	    continue;   #如果命名为ftrace.o就跳过.

2,verbose
    a,log from make,do mcount.
    b,if [ "-pg" = "-pg" ]; then
	if [ /home/pangwz/noteGit/kernel/debug/ftrace/ftrace-deamon.o != "scripts/mod/empty.o" ]; then
	    ./scripts/recordmcount  "/home/pangwz/noteGit/kernel/debug/ftrace/ftrace-deamon.o";
	fi;
    fi;

3,use ref
    debug/traceprintk/readme.txt
    debug/traceprintk/readme.txtdd
    [root@linux tracing]# echo 1 > /proc/sys/kernel/ftrace_enabled
    [root@linux tracing]# echo function_graph > current_tracer # 事先加载模块 ftrace_demo (加载后才能在写文件 set_ftrace_filter 时找到该模块)
    [root@linux tracing]# echo ':mod:ftrace_demo' > set_ftrace_filter
    [root@linux tracing]# cat set_ftrace_filter ftrace_demo_init ftrace_demo_exit # 将模块 ftrace_demo 卸载
    [root@linux tracing]# echo 1 > tracing_enabled # 重新进行模块 ftrace_demo 的加载与卸载操作
    [root@linux tracing]# cat trace
        # tracer: function_graph
        #
        # CPU  DURATION                  FUNCTION CALLS
        # |     |   |                     |   |   |   |
        1)| /* Can not see this in trace unless loaded for the second time */
        0)| /* Module unloading */

    原文链接：https://blog.csdn.net/panhewu9919/article/details/103114321/
