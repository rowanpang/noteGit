1,指针在做加/减算术运算时，会考率自身类型从而给出类型相对偏移量，而非地址之差. exp：
	int a[5];
		&a[4] - &a[0],给出的差值为4.
		*(a+2) == a[2];
	
	int *p = &a[4];
		*(p-1) == a[3];

2,内核充分使用了此机制
	init/main.c
		start_kernel(...)
			-->after_dashes = parse_args("Booting kernel",
			                  static_command_line, __start___param,
								__stop___param - __start___param,
								  -1, -1, NULL, &unknown_bootoption);

			__stop___param - __start___param，即param的数量.


