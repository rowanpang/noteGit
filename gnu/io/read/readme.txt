1,
    python -c "print 136*A+x54x05x40x00x00x00x00x00" | ./read
    echo -e "\x30"

2,std
    a,stdout redirect to fd1.txt

3,stdin
    a,read from terminos read是否直接返回取决于termios的设置.
    b,默认
	termios 工作在 canonical mode,
	输入按行有效
