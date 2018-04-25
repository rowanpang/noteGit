1,分治算法
    a,重点是划分出子问题,从而折半嵌套.

2,Time complexity

    T(n) = aT(n/b) + Θ(n^k)
	    / Θ(nlogba)	    if a > b^k
	 =  | Θ(n^klgn)	    if a = b^k
            \ Θ(n^k)	    if a < b^k

    T(n) = T(n/2) + Θ(1)
	 = T(n/2) + c
	 = c*lgn
	 = Θ(lgn)

    T(n) = T(n/2) + Θ(n)
	 = Θ(n)

    T(n) = 2T(n/2) + Θ(n)
	 = Θ(nlgn)
