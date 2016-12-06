1,two type
	main.s
		-->call msg.c  msg(31)
	main.s
		-->call msg.c _msg(31)

2,underscore prefix
	prefix 是gcc编译工程中自动添加的.有 -fleading-underscore 编译选项控制
	ref Makfile

	需要指出,如果underscore msg(xx) 会提示找不到_printf,因为printf是非underscore的.
	所以，要在msg.c中强制声明，extern int print(xxxxx) asm("printf")
