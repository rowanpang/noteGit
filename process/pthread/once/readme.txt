$ ./once                                                                                                                                                                                   (master|…1)
    in main,tid:1222025,pid:1222025,parent:1219818
    once func will run by main thread                               #主线程中执行.
    in func once_init,tid:1222025
    tid:1222026,parent:1219818
    main exit,ret:0

$ ./once asdkfj                                                                                                                                                                            (master|…1)
    in main,tid:1222064,pid:1222064,parent:1219818
    once func will run by non main thread
    tid:1222065,parent:1219818
    in func once_init,tid:1222065                                   #子线程中执行
    main exit,ret:0

