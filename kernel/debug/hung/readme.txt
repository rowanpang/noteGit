1,insmod
	echo 60 > /proc/sys/kernel/hung_task_time_out
2,wait
	等待,ko 将导致hung task
3,rmmod
	kthread 将被唤醒,并销毁.系统恢复正常.
