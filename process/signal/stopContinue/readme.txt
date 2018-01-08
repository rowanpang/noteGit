1,sigtstp
    a,flow
	./sigtstp
	ctrl+z to stop the task
	kill -CONT `pidof sigtstp`	#send continue signal

    b,result
	sigtstp will run in background.
	but the stdio is not change. so still see mesg.

    c,fg 1
	make sigtstp foreground

2,kill -INT  无法唤醒stop状态的task.
    a,flow
	./sigtstp
	ctrl+z  stop the task
	kill -INT `pidof sigtstp`
	    #并没有唤醒task,SIGINT没有处理.
	Kill -CONT `pidof sigtstp`
	    #唤醒task,处理SIGINT tsk Interrupt.

3,kill -9
    a,可以唤醒处于stop的tsk 并kill掉.
    b,flow
	./sigtstp
	ctrl+z  stop the task
	kill -9 `pidof sigtstp`
