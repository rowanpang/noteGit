.pushsection .smp_locks, "a"
.balign 4
.byte 2,2,2,2 
.long 1f - .
.long 2f - 1f 
1:.long 3,4,5
.long 3,4,5
.byte 1,1,1,1 
2:.long 7,8
.popsection
