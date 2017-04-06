1,about file name
    a,不用使用ftrace.c,在生成_mcount_loc section时会跳过这个命名的文件.
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

3,
