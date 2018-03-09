1,cgi 配置
    <Directory "/var/www/html/shellCGI">
	Options +ExecCGI
	Options Indexes FollowSymLinks ExecCGI
	AllowOverride None
	Require all granted
	AddHandler cgi-script .exe .pl .cgi .sh
    </Directory>

2,如果是不cgi目录则类似与下载shell文件,不会在server端执行.

3,mv hello.sh /var/www/cgi-bin/
