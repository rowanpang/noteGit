0,basic
    a,bash 转义ref
        https://www.cnblogs.com/ziyunfei/p/4779441.html#:~:text=Bash%20%E9%87%8C%E7%9A%84%E8%BD%AC%E4%B9%89%E5%BA%8F%E5%88%97%20%E5%9C%A8%20Bash%20%E9%87%8C%EF%BC%8C%E4%B8%80%E5%85%B1%E6%9C%89%E4%BA%94%E4%B8%AA%E5%9C%B0%E6%96%B9%E6%94%AF%E6%8C%81%E5%8F%8D%E6%96%9C%E6%9D%A0%E5%BC%80%E5%A4%B4%E7%9A%84%E8%BD%AC%E4%B9%89%E5%BA%8F%E5%88%97%EF%BC%8C%E5%8C%85%E6%8B%AC%E4%B8%A4%E4%B8%AA%E5%86%85%E9%83%A8%E5%91%BD%E4%BB%A4%20echo%20%E5%92%8C,printf%20%E7%9A%84%E5%8F%82%E6%95%B0%E9%87%8C%EF%BC%8C%E5%AD%97%E7%AC%A6%E4%B8%B2%E8%AF%AD%E6%B3%95%20%24%27...%27%20%E9%87%8C%EF%BC%8C%E8%BF%98%E6%9C%89%E5%9B%9B%E4%B8%AA%E6%8F%90%E7%A4%BA%E7%AC%A6%E5%8F%98%E9%87%8F%20PS1-PS4%20%E9%87%8C%EF%BC%8C%E4%BB%A5%E5%8F%8A%E5%9C%A8%20Readline%20%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6%E9%87%8C%EF%BC%88%E7%94%A8%E6%9D%A5%E8%87%AA%E5%AE%9A%E4%B9%89%E9%94%AE%E7%9B%98%E5%BF%AB%E6%8D%B7%E9%94%AE%EF%BC%89%E3%80%82

1,main_gbk.c
    为gbk编码，经过gcc生成a.out时，其中的字符编码依然为gbk.
    set fileencoding?
    fileencoding=cp936

2,创建文件,将内容写入到其中后,因为写入内容是gbk编码的所以
	说文件是gbk编码的.

3, 庞
	gbk: 0xc5d3
	"p\305\323\316\260\325\361"

4,with fat
	a,因为文件名时gbk编码，所以mounts时需要制定对应的iocharset=cp936,这样才不会出错.
	b,而在linux下如果想要正常列出文件名，iocharset 应该为 utf8,这是一个冲突项.
