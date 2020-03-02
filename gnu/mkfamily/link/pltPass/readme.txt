1,libhello.so
    a,show.c show2.c hello.c  编译生成.
    b,hello.c  call show() in show.c

2,a.out
    a,libhello.so main.c 编译生成.
    b,main.c all
	sayhello()  in hello.c
	show()	    in show.c
	show2()	    in show2.c

3,pltPass 的两层含义:
    a,在libhello.so 内部调用内部函数时不要走plt,而是直接使用内部的.
    b,不能影响外部对内部函数的调用.
