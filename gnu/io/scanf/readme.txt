1,basic
    a,根据format来处理输入.
	format 有3中指令:
	    1),A  sequence  of  white-space characters,
		匹配任意个空格.
	    2),An ordinary character (i.e., one other than white space or '%')
		以字符.must exactly match.
	    3),A conversion specification
		'%xx',格式声明.

    b,%s
	将匹配字符,直到下一个wite-space character.
