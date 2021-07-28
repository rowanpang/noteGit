0,basic
    a,server
        nc -lk
    b,client
        ./client

1,客户端
    $ ./client                                                                             (master|…1)
    before connect
    input char to send, no to stop
    a
    ret 2
    input char to send, no to stop
    b
    ret 2
    input char to send, no to stop
    c
    ret 2
    input char to send, no to stop
    d
    ret 2
    input char to send, no to stop
    e                                                   #发送e之前,调整server iptables, iptables -I OUTPUT 1 -p tcp --sport 31337 --jump DROP. cli无法收到ack,一直需要重传.
    ret 2
    input char to send, no to stop
    f
    ret 2                                               #'f\n'2个字符可以正常send, 没有报错. 'e\n' 在重传中
    input char to send, no to stop
    g
    ret 2
    when send: Connection timed out                     #send 'g\n' 时返回错误,重传超时了. 重传超时会被延时检测到.
