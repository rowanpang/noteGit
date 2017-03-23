1,diff
    2.sh
	sleep 30 && ./3.sh	#需要等待sleep 进程的返回.
    2.daemon.sh
	sleep 30 &		#sleep 放到后台,不影响前台进程的执行.
	./3.sh


2,从1.sh看
     ./2.daemon.sh
     ./2.sh
     只有2.daemon.sh 返回后才会执行2.sh
