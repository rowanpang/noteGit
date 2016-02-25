KERNELDIR ?=/home/student/linux-2.6.32.2
all: memdev_test 

memdev_test : memdev_test.c
	#arm-linux-gcc -I$(KERNELDIR) -s -Wl,-warn-common --static -o $@ $^
	arm-linux-gcc -I$(KERNELDIR) -o $@ $^

clean :
	rm memdev_test
