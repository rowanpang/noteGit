1,dlopen
    a,如果filename种包含'/',则被认为是相对路径 or 绝对路径.  按照此路径查找.
    b,如果仅仅是name. 则按照ld.so的加载规则自动查找.exp:
	1),msg.so   及 LD_LIBRARY_PATH
	2),则会在LD_LIBRARY_PATH 路径下查找msg.so 文件.

2,测试方法:
    make
    执行 run.sh

3,运行种改变LD_LIBRARY_PATH 环境变量,对so的加载路径无效.
