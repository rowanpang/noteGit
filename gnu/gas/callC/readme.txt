1,
	main.s
		-->call msg.c  msg(31)

	main.s 中使用
		call msg  而非  call _msg
	
2,
	linux v0.12 中asm call c 使用的是 _msg ?? why?
