1,use
	a,insmod xxx.ko spid=xx epid=xx
		spid:start pid
		epid:end pid
	b,insmod xxx.ko cpid="3,[45,90],6,7"
		pid: 3,6,7,start 45,end 90[include 45,90] will be show sid
