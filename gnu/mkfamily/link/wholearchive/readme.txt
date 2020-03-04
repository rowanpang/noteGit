0,basic
    a,编译流程
	hello.o + msg.o = libhellomsg.a
	dyn.o 依赖于msg(), + libhellomsg.a = libdyn.so
	dyn.o 依赖于msg(), + libhellomsg.a all archive = libdyn.all.so

	main.o 依赖于 msg(),dyn(),sayHello() + libdyn.so = dynelf
	main.o 依赖于 msg(),dyn(),msg_direct() + libdyn.so = dynelf.msg.direct
	main.o 依赖于 msg(),dyn(),sayHello() + libdyn.all.so = dynelf.all

    b,编译结果
	dynelf 报错, undefined reference to `sayHello'

    c,得到结论
	.o or .a 链接为.so 时
	    仅仅将需要的symbol对应object file 引入,不需要的object file 不引入
	    这里是引入整个object file ,而不是单单所依赖的symbol. vs dynelf and dynelf.msg.direct
	dyn.o 不依赖与 hello.o中的任何symbol 所有不会引入.


1,link order
    $ gcc -shared libhellomsg.a dyn.o -o libdyn.so ; nm libdyn.so | grep msg
	    U msg
    $ gcc -shared -Wl,--start-group libhellomsg.a -Wl,--end-group dyn.o -o libdyn.so ; nm libdyn.so | grep msg
	    U msg
	#上面2种情况,dyn.o 后面找不到依赖的msg(),所以msg()依然标记为undefined.

    $ gcc -shared dyn.o libhellomsg.a -o libdyn.so ; nm libdyn.so | grep msg
	0000000000000673 T msg
    $ gcc -shared dyn.o -Wl,--start-group libhellomsg.a -Wl,--end-group -o libdyn.so ; nm libdyn.so | grep msg
	0000000000000673 T msg
	#简单的将dyn.o 放到 libhellomsg.a 前面即可.

    $ gcc -shared -Wl,--start-group libhellomsg.a dyn.o -Wl,--end-group -o libdyn.so ; nm libdyn.so | grep msg
	0000000000000673 T msg
	#将dyn.o 与 libhellomsg.a 放到同一个group中,可以重复来回搜索msg(),可以找到symbol
	#放到同一个group中更多时候用法为
	    gcc -shared dyn.o -Wl,--start-group xa.o xb.o libxa.a libxb.so libxc.a -Wl,--end-group -o libdyn.so

