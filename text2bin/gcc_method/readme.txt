1,make format in.txt.c
	use vim

2,gcc -c in.txt.c 
	got in.txt.o

3,objcopy .data section
	objcopy -j .data -O binary in.txt.o in.txt.o.data		

4,drop the last '0x00'
	open("out.bin","w").write(open("in.txt.o.data","r").read(643))
	open("out.bin2","w").write(open("in.txt.o.data","r").read(os.path.getsize("in.txt.o.data")-1))
