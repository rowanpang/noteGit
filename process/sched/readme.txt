0,cmd
    (./niceTest&); (./niceTest 19&); (./niceTest 10&); (./niceTest -5&); (./niceTest -20&);
    nice -5 时
	不容易看出 nice的效果.
	已经能独占一个cpu,cpu使用率100%.
    对比nice 10,nice -20 就能看出cpu 利用率不同.
