a,hello.o , foo.o:
	hello.o  调用 foo.o中实现的函数及全局变量.即 hello.o 与 foo.o一起可以连接为a.out可执行文件

b,hello.o.fPIC / foo.o.fPIC:
	使用fPIC flag 编译出的模块.

c,模块：
	指一个*.o,模块外部即*.o之外

d,a.out / a.out.hello.fPIC / a.out.foo.fPIC / a.out.fPIC
	hello.o + foo.o -->a.out
	hello.o.fPIC + foo.o ---->a.out.hello.fPIC
	.........
	hello.o.fPIC + foo.o.fPIC ------>a.out.fPIC	

