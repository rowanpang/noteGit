1,insertion
    前j-1个元素已经排好了，现在需要排a[j],则
    从a[j-1]开始向前,依次向后移动a[i],直到 a[i] < a[j],将
    a[j] 放入 a[i+1],即
    找到一个放a[j]的地方,将a[j]插入进去,故名 插入法.

2,merge
    a,Time complexity
	T(n) = 2T(n/2) + Θ(n)
	    2T(n/2):表示2个递归调用时间相加
	    Θ(n):表示merge操作的时间,merge里面没有循环嵌套
		只是1重循环,所以消耗时间为线性

	T(n) = 2T(n/2) + cn, c is a constant,使用recursive tree 方法
	T(n) = cn*lgn + Θ(n)
	     = Θ(nlgn)

    b,merge sort always divides the array in two halves and
	take linear time to merge two halves.
