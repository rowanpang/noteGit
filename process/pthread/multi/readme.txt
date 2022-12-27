shell1:
    $ ./multi
shell2:
    $ stap --poison-cache -v escape.stp

OR

$ stap --poison-cache -v escape.stp -c ./multi                                                                                                                                             (master|…2)
    Pass 1: parsed user script and 503 library scripts using 333668virt/98232res/12376shr/85516data kb, in 180usr/20sys/193real ms.
    Pass 2: analyzed script: 5 probes, 4 functions, 1 embed, 3 globals using 335508virt/101576res/13684shr/87356data kb, in 10usr/0sys/8real ms.
    Pass 3: translated to C into "/tmp/staprBbTBc/stap_2313e6d178fc72ae8db44ac8803971c5_3227_src.c" using 335772virt/101980res/13812shr/87620data kb, in 10usr/80sys/94real ms.
    Pass 4: compiled C into "stap_2313e6d178fc72ae8db44ac8803971c5_3227.ko" in 14900usr/1970sys/6601real ms.
    Pass 5: starting run.
    in main:419816,parent:419814
    idx:000,tid:419822,parent:419814
    idx:001,tid:419823,parent:419814
    ......
    in main join thidx:000
    idx:081,tid:419903,parent:419814
    idx:086,tid:419908,parent:419814

      value |-------------------------------------------------- count
     800000 |                                                     0
     900000 |                                                     0
    1000000 |@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                  101
    1100000 |                                                     0
    1200000 |                                                     0

      value |-------------------------------------------------- count
     800000 |                                                     0
     900000 |                                                     0
    1000000 |@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@        301
    1100000 |                                                     0
    1200000 |                                                     0

      value |-------------------------------------------------- count
     800000 |                                                     0
     900000 |                                                     0
    1000000 |@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@      501
    1100000 |                                                     0
    1200000 |                                                     0

      value |-------------------------------------------------- count
     800000 |                                                     0
     900000 |                                                     0
    1000000 |@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     701
    1100000 |                                                     0
    1200000 |                                                     0

      value |-------------------------------------------------- count
     800000 |                                                     0
     900000 |                                                     0
    1000000 |@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    901
    1100000 |                                                     0
    1200000 |                                                     0

    Pass 5: run completed in 10usr/40sys/10371real ms.

$
