1,basic
    a,server 得到client上bash shell 在server控制client.
    b,反向链接，reverse shell.

2,部署
    a,server:
	在一个公网ip的机子上,  等待client的连接.
	可以直接使用nc -l xxx 代替.

    b,client:
	链接server,提供shell 给server.
