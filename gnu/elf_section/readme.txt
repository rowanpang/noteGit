1, 修改section的flag
	psetcion 默认是rw的，分配到rw segment，通过修改section 为readonly，将其分配到readonly的segment
	gcc -c main.c
	objcopy --set-section-flag .psection="alloc,readonly" main.o 
	gcc main.o 生成a.out

	a.out 执行时因为写只读，segment error


2,向a.out中添加section
	gcc main.c 生成a.out
	objcopy --add-section .note.module.sign=sign.o a.out a.out.n


