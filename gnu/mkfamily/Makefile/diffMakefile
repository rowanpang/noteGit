.PHONY:all
all:a.out a.strip af

a.out:
	@gcc -g main.c -o a.out
	@readelf -aW a.out > elf.log
	@objdump -asSd a.out > objdump.log

a.strip:a.out
	@strip a.out -o a.strip
	@readelf -aW a.strip > elf.strip.log
	@objdump -asSd a.out > objdump.strip.log

.PHONY:af
af:a.out a.strip
	$(shell diff elf.log elf.strip.log > elf.diff.log)
#diff elf.log elf.strip.log > elf.diff.log will be error  ---------> pangwz

clean:
	@rm a.out elf.log objdump.log
	@rm a.strip elf.strip.log objdump.strip.log
	@rm elf.diff.log

