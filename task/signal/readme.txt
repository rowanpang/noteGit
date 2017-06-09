1,sigtstp
    a,flow
	./sigtstp
	ctrl+z to stop the task
	kill -CONT `pidof sigtstp`

    b,result
	sigtstp will run in background.
	but the stdio is not change. so still see mesg.

    c,fg 1
	make sigtstp foreground
