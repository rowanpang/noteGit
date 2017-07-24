1,basic
    a,fortify source 是与gcc optimize 一起使用的.
    b,主要是优化可以在编译时确认内存大小的情况.

2,exp poll --> fortify __poll_chk
    a,define  in poll2.h
	poll (struct pollfd *__fds, nfds_t __nfds, int __timeout)
	{
	    if (__bos (__fds) != (__SIZE_TYPE__) -1) {				    #fdset 是否有效，!NULL.
	      if (! __builtin_constant_p (__nfds))				    #编译时是否可以确认 nfds,即fdnum 为常量.
		return __poll_chk (__fds, __nfds, __timeout, __bos (__fds));		#非常量,编译时无法check, check when running, call chk.
	      else if (__bos (__fds) / sizeof (*__fds) < __nfds)			#常量,编译时即可检测正确性即判断出是否会overflow.判断为overfolw 则 call warn
		return __poll_chk_warn (__fds, __nfds, __timeout, __bos (__fds));  
	    }

	    return __poll_alias (__fds, __nfds, __timeout);				#常量,不会overflow 则call这里
	}

    b,myPoll2(xx)
	a,在编译时无法确认内存大小,也就无法进行判断.
