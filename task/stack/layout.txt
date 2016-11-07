layout:
	stack segment:
			segment do_execev 过程中自动分配,vma 每次执行可能不同.
			start 			end				size
	  0x7ffffffde000     0x7ffffffff000    0x21000          0                           [stack] 
	+--------+--------------------+  f000
    |        |                    |
    |        |                    |
    |        |                    |
	+--------+--------------------+
    |        |     't'            |
	+        +--------------------+  eff6   ref:do_execve.txt:copy_strings_kernel()
    |        | "/home/xxxx/a.ou"  |
    +--------+--------------------+  efd5 "/home/pangez/xxxx/a.out" 
    |     binary = '0000'         |  
	+--------+--------------------+  efd4
    | env[n] | "COLOR.....al"     |
	+--------+--------------------+  efbc  "COLORTERM=gnome-terminal"  //pangwz:最后一个env  ref: 下面 cat /proc/16356/environ ---\
    | env[*] |     '*'            |                                                                                                |
	+--------+--------------------+  xxxx                                                                                          |
    |        |     '*'            |                                                                                                |
	+ env[0] +--------------------+  exxx                                                                                          |
    |        |     'O'            |                                                                                                |
    +--------+--------------------+  e4b5   "ORBIT_SOCKETDIR=/tmp/orbit-pangwz" <-----                                             |
    |     binary = '0000'         |                                                   \                                            |
    +--------+--------------------+  e4b4                                             |                                            |
    |        |     'o'            |                                                   |                                            |
    +        +--------------------+  e4b3                                             |                                            |
    |        |     '*'            |                                                   |                                            |
    + arg[1] +--------------------+                                                   |                                            |
    |        |     'a'            |                                                   |                                            |
    +        +--------------------+                                                   |                                            |
    |        |     'p'            |                                                   |                                            |
    +--------+--------------------+  e4a9 "pangwzHello" <-------------------------    |                                            |
    |     binary = '0000'         |                                               \   |                                            |
    +--------+--------------------+  e4a8                                         |   |                                            |
    |        |     't'= 0x74      |                                               |   |                                            |
    +        +--------------------+  e490                                         |   |                                            |
    |        |     '*'            |                                               |   |                                            |
    + arg[0] +--------------------+  exxx                                         |   |                                            |
    |        |     'o'            |                                               |   |                                            |
    +        +--------------------+  e487                                         |   |                                            |
    |        |     'h'            |                                               |   |                                            |
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+  e486 "/home/pangez/xxxx/a.out"  <--------    |   |                                            |
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                                           \   |   |                                            |
    +~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+                                           |   |   |                                            |
    +--------+--------------------+                                           |   |   |                                            |
    |    binary = '0x21'          |                                           |   |   |                                            |
    +--------+--------------------+   +8=e330                                 |   |   |                                            |
    |    binary = '0000'          |                                           |   |   |                                            |
    +--------+--------------------+   +8=e328                                 |   |   |                                            |
    |        |  env[n]='addr'     |                                           |   |   |                                            |
    +        +--------------------+   +n=e320                                 |   |   |                                            |
    | env    |  ***************   |                                           |   |   |                                            |
    +        +--------------------+                                           |   |   |                                            |
    |        |  env[0]=e4b5 ------+-------------------------------------------+---+--/                                             |
    +--------+--------------------+   +8=e1b0                                 |   |                                                |
    |      binary='000'           |                                           |   |                                                |
    +--------+--------------------+   +8=e1a8                                 |   |                                                |
    |        | &arg[1]= e4a9 -----+-------------------------------------------+--/                                                 |
    + argv   +--------------------+   +8=e1a0                                 |                                                    |
    |        | &arg[0]= e486 -----+------------------------------------------/-----------------------------------------------------+-------------------------- 
    +--------+--------------------+--->e198                                                                                        |                          \ 
                                                                                                                                   |                           \
                                                                                                                                   |                           |
                                                                                                                                   |                           |
gdb ./a.out                                                                                                                        |                           |
	(gdb) set args "pangwzHello"   内核中将看不到'"',pangwz                                                                        |                           |
	(gdb) show args                                                                                                                |                           |
		Argument list to give program being debugged when it is started is ""pangwzHello"".                                        |                           |
	(gdb) break main                                                                                                               |                           |
		Breakpoint 1 at 0x400536: file main.c, line 19.                                                                            |                           |
	(gdb) r                                                                                                                        |                           |
		Starting program: /home/pangwz/test/task/stack/a.out "pangwzHello"                                                         |                           |
		Breakpoint 1, main (argc=2, argv=0x7fffffffe198) at main.c:19                                                              |                           |
		19int ret = 0;                                                                                                             |                           |
	(gdb) info proc mapping                                                                                                        /                           |
		process 16356                        <------------------------------------------------------------------------------------\                            |
		cmdline = '/home/pangwz/test/task/stack/a.out'                                                                             \                           |
		cwd = '/home/pangwz/test/task/stack'                                                                                        \                          |
		exe = '/home/pangwz/test/task/stack/a.out'                                                                                   \                         |
		Mapped address spaces:                                                                                                        \                        |
                                                                                                                                       \                       |
				  Start Addr           End Addr       Size     Offset objfile                                                           |                      |
					0x400000           0x401000     0x1000          0                              /home/pangwz/test/task/stack/a.out   |                      |
					0x600000           0x601000     0x1000          0                              /home/pangwz/test/task/stack/a.out   |                      |
				0x3a56e00000       0x3a56e20000    0x20000          0                         /lib64/ld-2.12.so                         |                      |
				0x3a5701f000       0x3a57020000     0x1000    0x1f000                         /lib64/ld-2.12.so                         |                      |
				0x3a57020000       0x3a57021000     0x1000    0x20000                         /lib64/ld-2.12.so                         |                      |
				0x3a57021000       0x3a57022000     0x1000          0                                                                   |                      |
				0x3a57200000       0x3a5738a000   0x18a000          0                         /lib64/libc-2.12.so                       |                      |
				0x3a5738a000       0x3a5758a000   0x200000   0x18a000                         /lib64/libc-2.12.so                       |                      |
		---Type <return> to continue, or q <return> to quit---                                                                          |                      |
				0x3a5758a000       0x3a5758e000     0x4000   0x18a000                         /lib64/libc-2.12.so                       |                      |
				0x3a5758e000       0x3a5758f000     0x1000   0x18e000                         /lib64/libc-2.12.so                       |                      |
				0x3a5758f000       0x3a57594000     0x5000          0                                                                   |                      |
			  0x7ffff7fdd000     0x7ffff7fe0000     0x3000          0                                                                   |                      |
			  0x7ffff7ffa000     0x7ffff7ffb000     0x1000          0                                                                   |                      |
			  0x7ffff7ffb000     0x7ffff7ffd000     0x2000          0                           [vvar]                                  |                      |
			  0x7ffff7ffd000     0x7ffff7fff000     0x2000          0                           [vdso]                                  |                      |
			  0x7ffffffde000     0x7ffffffff000    0x21000          0                           [stack]                                 |                      |
		  0xffffffffff600000 0xffffffffff601000     0x1000          0                   [vsyscall]                                      |                      |
	(gdb) info stack                                                                                                                    |                      |
		#0  main (argc=2, argv=0x7fffffffe198) at main.c:19                                                                             |                     /
	(gdb) x/2xg 0x7fffffffe198                                                                                                          |                    /
		0x7fffffffe198:	0x00007fffffffe486	0x00007fffffffe4a9     ---------------------------------------------------------------------+-------------------/
	(gdb) x/s 0x00007fffffffe486                                                                                                        |
		0x7fffffffe486:	 "/home/pangwz/test/task/stack/a.out"                                                                           |
	(gdb)                                                                                                                               |
		0x7fffffffe4a9:	 "pangwzHello"                                                                                                  |
	(gdb)                                                                                                                               |
		0x7fffffffe4b5:	 "ORBIT_SOCKETDIR=/tmp/orbit-pangwz"                                                                            |
	(gdb) next                                                                                                                          |
		22		printf("in main arg[0]:%s %#x\n",argv[0],&argv[0]);                                                                     |
	(gdb) next                                                                                                                          |
		in main arg[0]:/home/pangwz/test/task/stack/a.out 0xffffe198                                                                    |
		23		printf("in main sVar:%#x=%d,gVar:%#x=%d \n",sVar,sVar,gVar,gVar);                                                       |
	(gdb) x/s 0x7fffffffef84                                                                                                            |
        0x7fffffffef84:  "XAUTHORITY=/var/run/gdm/auth-for-pangwz-2ddbqF/database"                                                      |
	(gdb)                                                                                                                               |
		0x7fffffffefbc:  "COLORTERM=gnome-terminal"                                                                                     |
                                                                                                                                        |
                                                                                                                                        |
                                                                                                                                        /
env:                                                                                                                                   / 
	[pangwz@rowanInspur ~]$ cat /proc/16356/environ              <--------------------------------------------------------------------- 
		ORBIT_SOCKETDIR=/tmp/orbit-pangwzSSH_AGENT_PID=4935HOSTNAME=rowanInspurIMSETTINGS_INTEGRATE_DESKTOP=yesSHELL=/bin/bashTERM=xtermXDG_SESSION_COOKIE=f84aa454d18e8e23989995f200000022-1477876085.329835-808719621HISTSIZE=1000WINDOWID=33713956QTDIR=/usr/lib64/qt-3.3QTINC=/usr/lib64/qt-3.3/includeIMSETTINGS_MODULE=IBusUSER=pangwzLS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=01;05;37;41:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arj=01;31:*.taz=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.zip=01;31:*.z=01;31:*.Z=01;31:*.dz=01;31:*.gz=01;31:*.lz=01;31:*.xz=01;31:*.bz2=01;31:*.tbz=01;31:*.tbz2=01;31:*.bz=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.rar=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.jpg=01;35:*.jpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.axv=01;35:*.anx=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=01;36:*.au=01;36:*.flac=01;36:*.mid=01;36:*.midi=01;36:*.mka=01;36:*.mp3=01;36:*.mpc=01;36:*.ogg=01;36:*.ra=01;36:*.wav=01;36:*.axa=01;36:*.oga=01;36:*.spx=01;36:*.xspf=01;36:SSH_AUTH_SOCK=/tmp/ssh-HOEsHk4914/agent.4914GNOME_KEYRING_SOCKET=/tmp/keyring-8t7oYj/socketUSERNAME=pangwzCOLUMNS=199DESKTOP_SESSION=i3MAIL=/var/spool/mail/pangwzPATH=/home/pangwz/golang:/home/pangwz/golang/bin:/tools/arm/arm-2012.09/bin:/home/pangwz/golang:/home/pangwz/golang/bin:/tools/arm/arm-2012.09/bin:/usr/lib64/qt-3.3/bin:/usr/local/bin:/usr/bin:/bin:/usr/local/sbin:/usr/sbin:/sbin:/home/pangwz/bin:/home/pangwz/vm:/usr/local/python3.5.1/bin:/home/pangwz/.nutstore/dist/bin:/home/pangwz/bin:/home/pangwz/vm:/usr/local/python3.5.1/bin:/home/pangwz/.nutstore/dist/bin_=/usr/bin/gdbQT_IM_MODULE=ibusPWD=/home/pangwz/test/task/stackXMODIFIERS=@im=ibusGDM_KEYBOARD_LAYOUT=usLANG=en_US.utf8GNOME_KEYRING_PID=4905GDM_LANG=en_US.utf8LINES=16GDMSESSION=i3HISTCONTROL=ignoredupsSSH_ASKPASS=/usr/libexec/openssh/gnome-ssh-askpassSHLVL=3HOME=/home/pangwzLOGNAME=pangwzQTLIB=/usr/lib64/qt-3.3/libCVS_RSH=sshDBUS_SESSION_BUS_ADDRESS=unix:abstract=/tmp/dbus-pDHUL3ae4x,guid=2c6fc40deac115df4e113c9100000087GOPATH=/home/pangwz/golangLESSOPEN=|/usr/bin/lesspipe.sh %sWINDOWPATH=1DISPLAY=:0GTK_IM_MODULE=ibusG_BROKEN_FILENAMES=1XAUTHORITY=/var/run/gdm/auth-for-pangwz-2ddbqF/databaseCOLORTERM=gnome-terminal[pangwz@rowanInspur ~]$
