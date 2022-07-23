0,basic
    a,depth ref Makefile

    b,ltrace 支持 main->so/ so->so/ so->xx->so 等多种call追踪.

1,usage
    a,直接执行等于 ltrace -e @MAIN (main binary/elfbin)
        $ ltrace ./main > /dev/null
            sayhello(1, 0x7ffde1af3fd8, 0x7ffde1af3fe8, 0x7f9f04da3598)                                                                = 30
            show(0x402010, 0x7f9f04dc9011, 0, 0)                                                                                       = 27
            show2(0x402019, 0x7f9f04dc9011, 0, 0)                                                                                      = 27
            +++ exited (status 0) +++

    b,attache 到pid
        $ ltrace -p 592090
            virThreadJobSet(0x55e4aefa5833, 0x55e4b08500b0, 0x55e4b08551b0, 0x7f5defffe920)                                            = 0
            virLogMessage(0x55e4aefcb150, 1, 0x55e4aefa5eb8, 0x3cf6)                                                                   = 0
            virNetServerClientGetPrivateData(0x55e4b08500b0, 0x7f5e17323410, 0x7f5defffe6f0, 0)                                        = 0x55e4b082a340

    c,LD_PRELOAD=/path/to/preload/xx.so ltrace -l 'xx.so' ./elfbin -xx
        追踪PRELOAD库中的调用.

2,filter
    a,-e/-l/-x 同时出现为'或'的关系,只是从不同角度来说

    b,-e
        以symbol为主体来trace.

        $ ltrace -e show ./main > /dev/null                         #trace show()的调用,不管caller是谁.
            libhello.so->show(0x7ffd3e2a5e70, 0x7f403ed8c000, 0, 4096)                                                                 = 30
            main->show(0x402010, 0x7f403ed82011, 0, 0)                                                                                 = 27
            +++ exited (status 0) +++

        $ ltrace -e show@'libhello.*' ./main > /dev/null            #trace libhello 中对show()的调用.
            libhello.so->show(0x7ffdd22bc9a0, 0x7fea1783f000, 0, 4096)                                                                 = 30
            +++ exited (status 0) +++

        $ ltrace -e @MAIN ./main > /dev/null                        #trace 所有MAIN binary(elfbin) 中的dyn调用.
            main->sayhello(1, 0x7ffc45305368, 0x7ffc45305378, 0x7efd927a8598)                                                          = 30
            main->show(0x402010, 0x7efd927ce011, 0, 0)                                                                                 = 27
            main->show2(0x402019, 0x7efd927ce011, 0, 0)                                                                                = 27
            +++ exited (status 0) +++

        $ ltrace -e show2 ./main > /dev/null
            libshow.so->show2(0x7ffd7b33bbb0, 0x7f131d9fb000, 0, 3072)                                                                 = 30
            libshow.so->show2(0x402010, 0x7f131d9fb000, 0, 3072)                                                                       = 27
            main->show2(0x402019, 0x7f131d9f6011, 0, 0)                                                                                = 27
            +++ exited (status 0) +++

        $ ltrace -e show2@'libshow.*' ./main > /dev/null            #trace libshow 中对show2()的调用.
            libshow.so->show2(0x7ffffa082fb0, 0x7f9000f39000, 0, 3072)                                                                 = 30
            libshow.so->show2(0x402010, 0x7f9000f39000, 0, 3072)                                                                       = 27
            +++ exited (status 0) +++

    c,-l
        trace 某个库中实现的symbol
        $ ltrace -l 'libshow2.*' ./main > /dev/null                 #trace 所有libshow2 中实现的sym
            libshow.so->show2(0x7ffd14191320, 0x7f64368de000, 0, 3072)                                                                 = 30
            libshow.so->show2(0x402010, 0x7f64368de000, 0, 3072)                                                                       = 27
            main->show2(0x402019, 0x7f64368d9011, 0, 0)                                                                                = 27
            +++ exited (status 0) +++

        $ ltrace -e show2@'libshow.*' -l 'libhello.*' ./main > /dev/null        #trace libshow中对show2()的调用 + 任意对libhello.*中实现的sym的调用
            main->sayhello(1, 0x7ffcd0443b48, 0x7ffcd0443b58, 0x7f207b400598 <unfinished ...>
            libshow.so->show2(0x7ffcd04439c0, 0x7f207b42b000, 0, 3072)                                                                 = 30
            <... sayhello resumed> )                                                                                                   = 30
            libshow.so->show2(0x402010, 0x7f207b42b000, 0, 3072)                                                                       = 27
            +++ exited (status 0) +++



