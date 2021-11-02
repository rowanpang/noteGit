0.baes
    a,对于一个so库，在libs 目录下一般为如下情况
        ll /lib64/libXi.so*
            lrwxrwxrwx. 1 root root    14 Feb  4  2016 /lib64/libXi.so -> libXi.so.6.1.0
            lrwxrwxrwx. 1 root root    14 Feb  4  2016 /lib64/libXi.so.6 -> libXi.so.6.1.0
            -rwxr-xr-x. 1 root root 64600 Feb  4  2016 /lib64/libXi.so.6.1.0
    b,其中
        /lib64/libXi.so 属于 libXi-devel-$ver rpm
        /lib64/libXi.so.6* 属于 libXi-$ver rpm
        libXi.so.6.1.0  记录soname为 readelf -aW /lib64/libXi.so.6.1.0 | grep SONAME
        0x000000000000000e (SONAME)             Library soname: [libXi.so.6]

    c,app 开发时使用libXi,提供 -lXi的支持
    d,app 开发环境下使用的是libXi 连接到的libXi.so.6.1.0,如何保证app 在运行环境中也可以使用这个库，
            或者与这个库向兼容的libXi.so.$ver 库?  (运行环境可能是 libXi.so -> libXi.so.5.1.0,即不同版本的so)

    e,soname的引入解决了
        1),精确体现了need 库的名字.
            高版本开发环境开发的app 不可以在底版本运行环境中运行.
            如果不是用soname，则运行时加载的是名为libXi.so的库，如果 ->libXi.so.5.xxx 则app可能运行失败.
            使用了soname,则运行时加载的是名为'soname'的库  ref down part 1,soname.out
        2),向后的兼容性
            底版本开发环境开发的app可以在高版本库的运行环境中运行.
            为了精确体现当然可以直接连接libXi.so.6.1.0,这样带来的后果是
            如果运行环境安装了libXi.so.6.2.0 （对某个fun做了优化的库),因为不存在libXi.so.6.1.0 则无法运行.

2,生成libfoo.so.1.1
    a,生成lib,编译过程中为lib库指定了so name.
        libfoo.so.1.1:foo.o
            @ld -shared foo.o -o libfoo.so.1.1 -soname=libfoo.so.1

    b,readelf -aW libfoo.so.1.1
            Dynamic section at offset 0x338 contains 14 entries:
                    Tag				   Type                         Name/Value
            0x000000000000000e (SONAME)             Library soname: [libfoo.so.1] <--------------------------\
                                                                                                              |
1,soname.out                                                                                                  |
    a,可执行文件依赖于lib库                                                                                   |
	@gcc hello.o -lfoo -o soname.1.out -Wl,-L. -Wl,-rpath=`pwd`/rpath                                     |
        -lfoo,指定了依赖的库                                                                                  |
        -Wl,-L.,指定了当前路径为link时的搜索目录                                                              |
        -Wl,-rpath=`pwd`/rpath, 指定runtime load时的库加载路径.                                               |
                                                                                                              |
    b,在soname中记录的NEED信息为:readelf -Wa                                                                  |
            Dynamic section at offset 0x7f8 contains 22 entries:                                              |
                    Tag		                    Type            Name/Value                                |
            0x0000000000000001 (NEEDED)             Shared library: [libfoo.so.1]<----------------------------+
            0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]                               |
            0x000000000000000f (RPATH)              Library rpath: [/home/pangwz/test/gnu/link/2o/rpath]      |
    c,所以在运行时会在rpath下找libfoo.so.1这个文件，即使用libfoo.so中的DT_SONAME作为文件名查找库文件.         |
        而不是libfoo.so.1.1 make时连接到的so.                                                                 |

    d,rpath 下的文件状态                                                   \                                  |
        libfoo.so.1.2 链接到了使用foo2.c编译出来的动态库                     ---------------------------------/
        [pangwz@rowanInspur 2o]$ ll rpath/
            total 0
            lrwxrwxrwx. 1 pangwz pangwz 40 Mar  9 14:44 libfoo.so.1 -> /home/pangwz/test/gnu/link/2o/libfoo.so.1.1
            lrwxrwxrwx. 1 pangwz pangwz 39 Mar  9 14:44 libfoo.so.2 -> /home/pangwz/test/gnu/link/2o/libfoo.so.1.2
        [pangwz@rowanInspur 2o]$

3,nosoname
    a,所链接的so为libfoo_nosoname，so中没有指定DT_SONAME,所以 NEED信息为so文件的名字
        Dynamic section at offset 0x7f8 contains 22 entries:
            Tag					 Type           Name/Value
        0x0000000000000001 (NEEDED)             Shared library: [libfoo_nosoname.so]
        0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]
        0x000000000000000f (RPATH)              Library rpath: [/home/pangwz/test/gnu/link/2o]
    b,运行时查找libfoo_nosoname.so
