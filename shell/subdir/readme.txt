1,base
    l0.sh 中
	-->./l1/l1.sh
	-->./l1/l2/l2.sh

2,
    a,如果l1.sh中想要使用lib.sh中的func or var,一定要 source ./lib.sh,
	即便l0.sh中已经source了.
    b,可能是因为shell new 了一个进程来执行l1.sh
    c,相当于在l1.sh的上向文中执行下lib.sh,导入相关环境.
