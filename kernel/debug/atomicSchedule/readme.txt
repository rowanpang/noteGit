1,此例并不能got atomic Schedule error.
    在使用schedule时已经做了兼容处理，disable掉了preempt,所以不会看到error.

2,在中断上下文中可以看到.
