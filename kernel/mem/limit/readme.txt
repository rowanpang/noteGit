0,basic
    kmalloc 申请大于 2^(MAX_ORDER-1) 的连续内存失败.
$insmod plimit.ko; rmmod plimit.ko ; dmesg -c                                                                                                                                        (master|…)
    [4069498.181374] plimits test in func:plimit_init
    [4069498.181376] page_size:0x1000,shift:12, MAX_ORDER:11, orderlimits:11
    [4069498.181376] kmalloc ok, size of page: 0
    [4069498.181376] kmalloc ok, size of page: 1
    [4069498.181378] kmalloc ok, size of page: 2
    [4069498.181381] kmalloc ok, size of page: 3
    [4069498.181383] kmalloc ok, size of page: 4
    [4069498.181384] kmalloc ok, size of page: 5
    [4069498.181386] kmalloc ok, size of page: 6
    [4069498.181387] kmalloc ok, size of page: 7
    [4069498.181391] kmalloc ok, size of page: 8
    [4069498.181397] kmalloc ok, size of page: 9
    [4069498.181408] kmalloc ok, size of page: 10
    [4069498.184616] plimits test in func :plimit_exit

$insmod plimit.ko orderlimits=12; rmmod plimit.ko ; dmesg -c                                                                                                                         (master|…)
    [4069528.724093] plimits test in func:plimit_init
    [4069528.724096] page_size:0x1000,shift:12, MAX_ORDER:11, orderlimits:12
    [4069528.724097] kmalloc ok, size of page: 0
    [4069528.724098] kmalloc ok, size of page: 1
    [4069528.724102] kmalloc ok, size of page: 2
    [4069528.724104] kmalloc ok, size of page: 3
    [4069528.724105] kmalloc ok, size of page: 4
    [4069528.724108] kmalloc ok, size of page: 5
    [4069528.724109] kmalloc ok, size of page: 6
    [4069528.724112] kmalloc ok, size of page: 7
    [4069528.724115] kmalloc ok, size of page: 8
    [4069528.724124] kmalloc ok, size of page: 9
    [4069528.724137] kmalloc ok, size of page: 10
    [4069528.724142] kmalloc error, size of page: 11
    [4069528.725956] plimits test in func :plimit_exit
$
