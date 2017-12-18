1,basic
    a,gun auto build system
	autoheader autoconf automake aclocal ..

    b,ref
	./flow.gv

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
