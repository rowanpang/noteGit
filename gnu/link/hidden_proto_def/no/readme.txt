gcc -shared -fPIC ./hello.c -o libhello.so

gcc main.c -L./ -lhello -Wl,-rpath=/home/pangwz/test/gcc/sec_link_

