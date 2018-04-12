1,basic
    a,format string attack
	fedora 26 x86_64 上测试NG,应该是glibc改正了.
	打印了rsi,rdx,rcx,r8,r9之后　直接跳到pre frame rsp 打印.

    b,ref
	https://www.cs.virginia.edu/~cr4bd/4630/S2017/slides/20170322-slides-1up-animated.pdf
	http://codearcana.com/posts/2013/05/02/introduction-to-format-string-exploits.html
	https://pdfs.semanticscholar.org/5473/8d6122d5e0410da5455ffe4aa5f6edb257d3.pdf
	https://crypto.stanford.edu/cs155/papers/formatstring-1.2.pdf
	http://www.cis.syr.edu/~wedu/Teaching/cis643/LectureNotes_New/Format_String.pdf
