.PHONY:all


SHLIB_NAME = libhello
SHLIB_MAJOR = 1
SHLIB_MINOR = 1
SHLIB_SONAME = ${SHLIB_NAME}.so.${SHLIB_MAJOR}
SHLIB_VER = ${SHLIB_NAME}.so.${SHLIB_MAJOR}.${SHLIB_MINOR}
SHLIB = ${SHLIB_NAME}.so

CFLAGS = -Wall -ggdb 

all:a.out

a.out:main.c ${SHLIB_SONAME} ${SHLIB}
	gcc ${CFLAGS} main.c -L./ -lhello -Wl,-rpath=`pwd`

SHLIB_CFLAGS = -fPIC

SHLIB_SRC = show.c 	\
	    show2.c 	\
	    hello.c
SHLIB_OBJ = ${SHLIB_SRC:%.c=%.o} 
SHLIB_ASM = ${SHLIB_SRC:%.c=%.s} 

${SHLIB} ${SHLIB_SONAME}:${SHLIB_VER}
	ln -sf $< $@

${SHLIB_VER}:${SHLIB_OBJ}
	gcc -shared -Wl,-soname=${SHLIB_SONAME} -Wl,--start-group $^ -Wl,--end-group \
	    -o $@

${SHLIB_OBJ}:%.o:%.c
	gcc ${CFLAGS} ${SHLIB_CFLAGS} -c $< -o $@

${SHLIB_ASM}:%.s:%.c
	gcc ${CFLAGS} ${SHLIB_CFLAGS} -S $< -o $@

.PHONY:clean
clean:
	@-rm a.out 
	@-rm libhello.so*
	@-rm *.s
	@-rm *.o
