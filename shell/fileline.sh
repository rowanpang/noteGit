写法一：
复制代码 代码如下:
#!/bin/bash
while read line
do
echo $line
done < filename(待读取的文件)


写法二：
复制代码 代码如下:
#!/bin/bash
cat filename(待读取的文件) | while read line
do
echo $line
done


写法三：
复制代码 代码如下:
for line in `cat filename(待读取的文件)`
do
echo $line
done
