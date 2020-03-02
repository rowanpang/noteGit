1,basic
    a,gun auto build system
	autoheader autoconf automake aclocal ..
    b,ref
	./flow.gv
    c,autoreconf 之后如果修改configure.ac 之后不再需要显示的执行autoreconfig
	直接执行make 既可以完成reconfig过程.
    d,make dist
	${name}-${ver}.tar.gz

2,scratch
    a,manual write
	Makefile.am configure.ac
    b,seperate do autoheader,autoconf,.. cmd gen correspond files.
    c,finaly do ./configure gen Makefile

3,autoreconfig
    a,manual write
	Makefile.am configure.ac
    b,./autoreconfig
	auto do auheader/auconfig/... cmd
    c,./configure
    d,make
