
2,libfoo.so
	readelf -aW libfoo.so
	Dynamic section at offset 0x338 contains 14 entries:
		Tag				   Type                         Name/Value
		0x000000000000000e (SONAME)             Library soname: [libfoo.so.1] <------------------------------- 
                                                                                                              \
1,soname                                                                                                      |
	a,链接到了制定so name的libfoo.so                                                                          |
	b,在soname中记录的NEED信息为:readelf -Wa                                                                  |
		Dynamic section at offset 0x7f8 contains 22 entries:                                                  |
			Tag					 Type                         Name/Value                                      |
			0x0000000000000001 (NEEDED)             Shared library: [libfoo.so.1]<----------------------------+
			0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]                               |
			0x000000000000000f (RPATH)              Library rpath: [/home/pangwz/test/gnu/link/2o/rpath]      |
	c,所以在运行时会在rpath下找libfoo.so.1这个文件，即使用libfoo.so中的DT_SONAME作为文件名查找库文件.         |
	d,rpath 下的文件状态                                                   \                                  |
		libfoo.so.1 链接到了使用foo2.c编译出来的动态库                      ---------------------------------/
		[pangwz@rowanInspur 2o]$ ll rpath/
		total 0
		lrwxrwxrwx. 1 pangwz pangwz 40 Mar  9 14:44 libfoo.so.1 -> /home/pangwz/test/gnu/link/2o/libfoo2.so
		lrwxrwxrwx. 1 pangwz pangwz 39 Mar  9 14:44 libfoo.so.1.bak -> /home/pangwz/test/gnu/link/2o/libfoo.so
		[pangwz@rowanInspur 2o]$ 
	e,体现了soname的兼容特性

3,nosoname
	a,所链接的so为libfoo_nosoname，so中没有指定DT_SONAME,所以 NEED信息为so文件的名字
		Dynamic section at offset 0x7f8 contains 22 entries:
			Tag					 Type                         Name/Value
			0x0000000000000001 (NEEDED)             Shared library: [libfoo_nosoname.so]
		    0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]
			0x000000000000000f (RPATH)              Library rpath: [/home/pangwz/test/gnu/link/2o]
	b,运行时查找libfoo_nosoname.so
			 
