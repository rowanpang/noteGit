----------WARNNING: this is an abnormal test suit

	d,if kernel configed as disable preempt,this suit ok
	    just for test 	    

	a,can work on smp,on up will crash,soft lockup
	b,thread
		A,got spinlock and sleep-->shedule()
		B,release the cpu for this task. 
			--->abnormal, spinlock disable preempt 
	
	c,threadA
		just as thread,but wait for the samelock
		
