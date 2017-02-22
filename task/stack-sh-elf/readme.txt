1,man 2 execve

2,
    $ ./execve ./myecho
           argv[0]: ./myecho
           argv[1]: hello
           argv[2]: world

    $ ./execve ./script
           argv[0]: ./myecho
           argv[1]: script-arg
           argv[2]: ./script
           argv[3]: hello
           argv[4]: world

    parser:
	do_execveat_common,
	    通过copy args strings 准备了:
	    world       arg2    new arg4
	    hello	arg1	new arg3
	    ./script    arg0    --deleted--
	load_script 中:
	    删除了
			arg0
	    增加了
			./script	new arg2
			script-arg	new arg1
			./myecho	new arg0	
