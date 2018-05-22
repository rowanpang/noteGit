1,test.txt test.hardlink 链接到了同意个inode 532158
2,test.symbollink symbollink 到 test.txt
3,cp** 分别为从test.txt 及 test.hardlink.txt copy的文件，也有自己的inode
	532161，532160

4,dir cpio
	cpio 为创建对test.txt相关文件创建的cpio包
	目录下的txt等为从cpio中解压出来的，可以看到test即test.hardlink依然保持了hardlink属性
