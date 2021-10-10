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
