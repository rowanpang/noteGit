#gcc helloword.s

.global main
.text
main:
	mov $1,%eax			#syscall write
	mov $1,%edi			#stdout
	mov $message, %rsi	#source string
	mov $len,%edx		#len 
	syscall

	mov $60,%eax
	xor %rdi,%rdi
	syscall

message:
	.ascii "Hello,pworldaaaaaaa\n"
	#len = . - message or
	.set len, .- message
