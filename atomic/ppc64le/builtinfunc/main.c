#include<stdio.h>
#include<stdlib.h>

int main(int argc,char** argv)
{
	int a=atoi(argv[1]);
	int *pa=&a;

	int b=0;
	int *pb=&b;

	int ret=1;

	/*
		gcc -g3 main.c -o main
		main.c: In function ‘main’:
		main.c:16:29: error: failure memory model cannot be stronger than success memory model for ‘__atomic_compare_exchange’
		  __atomic_compare_exchange_n(pa,pb,3,0,__ATOMIC_RELAXED,__ATOMIC_ACQUIRE);
									 ^
		main.c:17:29: error: invalid failure memory model for ‘__atomic_compare_exchange’
		  __atomic_compare_exchange_n(pa,pb,3,0,__ATOMIC_RELAXED,__ATOMIC_RELEASE);
									 ^
		main.c:22:29: error: invalid failure memory model for ‘__atomic_compare_exchange’
		  __atomic_compare_exchange_n(pa,pb,3,0,__ATOMIC_ACQUIRE,__ATOMIC_RELEASE);
									 ^
		main.c:27:29: error: invalid failure memory model for ‘__atomic_compare_exchange’
		  __atomic_compare_exchange_n(pa,pb,3,0,__ATOMIC_RELEASE,__ATOMIC_RELEASE);
									 ^
		main.c:32:29: error: invalid failure memory model for ‘__atomic_compare_exchange’
		  __atomic_compare_exchange_n(pa,pb,4,0,__ATOMIC_ACQ_REL,__ATOMIC_RELEASE);
									 ^
		main.c:33:29: error: invalid failure memory model for ‘__atomic_compare_exchange’
		  __atomic_compare_exchange_n(pa,pb,4,0,__ATOMIC_ACQ_REL,__ATOMIC_ACQ_REL);

		main.c:63:29: error: invalid failure memory model for ‘__atomic_compare_exchange’
		  __atomic_compare_exchange_n(pa,pb,4,0,__ATOMIC_SEQ_CST,__ATOMIC_RELEASE);
									 ^
		main.c:64:29: error: invalid failure memory model for ‘__atomic_compare_exchange’
		  __atomic_compare_exchange_n(pa,pb,4,0,__ATOMIC_SEQ_CST,__ATOMIC_ACQ_REL);

		main.c:49:29: error: failure memory model cannot be stronger than success memory model for ‘__atomic_compare_exchange’
		  __atomic_compare_exchange_n(pa,pb,4,0,__ATOMIC_RELAXED,__ATOMIC_SEQ_CST);
									 ^
		main.c:55:29: error: failure memory model cannot be stronger than success memory model for ‘__atomic_compare_exchange’
		  __atomic_compare_exchange_n(pa,pb,4,0,__ATOMIC_ACQUIRE,__ATOMIC_SEQ_CST);
									 ^
		main.c:61:29: error: failure memory model cannot be stronger than success memory model for ‘__atomic_compare_exchange’
		  __atomic_compare_exchange_n(pa,pb,4,0,__ATOMIC_RELEASE,__ATOMIC_SEQ_CST);
									 ^
		main.c:68:29: error: failure memory model cannot be stronger than success memory model for ‘__atomic_compare_exchange’
		  __atomic_compare_exchange_n(pa,pb,4,0,__ATOMIC_ACQ_REL,__ATOMIC_SEQ_CST);
									 ^
		make: *** [main] Error 1
		[root@xsky201 test]# 
	*/

	__sync_bool_compare_and_swap(pa,4,5);
	__atomic_compare_exchange_n(pa,pb,3,0,__ATOMIC_RELAXED,__ATOMIC_RELAXED);
	//__atomic_compare_exchange_n(pa,pb,3,0,__ATOMIC_RELAXED,__ATOMIC_ACQUIRE);
	//__atomic_compare_exchange_n(pa,pb,3,0,__ATOMIC_RELAXED,__ATOMIC_RELEASE);
	//__atomic_compare_exchange_n(pa,pb,4,0,__ATOMIC_RELAXED,__ATOMIC_SEQ_CST);
	printf("\n");

	__atomic_compare_exchange_n(pa,pb,3,0,__ATOMIC_ACQUIRE,__ATOMIC_RELAXED);
	__atomic_compare_exchange_n(pa,pb,3,0,__ATOMIC_ACQUIRE,__ATOMIC_ACQUIRE);
	//__atomic_compare_exchange_n(pa,pb,3,0,__ATOMIC_ACQUIRE,__ATOMIC_RELEASE);
	//__atomic_compare_exchange_n(pa,pb,4,0,__ATOMIC_ACQUIRE,__ATOMIC_SEQ_CST);
	printf("\n");

	__atomic_compare_exchange_n(pa,pb,3,0,__ATOMIC_RELEASE,__ATOMIC_RELAXED);
	__atomic_compare_exchange_n(pa,pb,3,0,__ATOMIC_RELEASE,__ATOMIC_ACQUIRE);
	//__atomic_compare_exchange_n(pa,pb,3,0,__ATOMIC_RELEASE,__ATOMIC_RELEASE);
	//__atomic_compare_exchange_n(pa,pb,4,0,__ATOMIC_RELEASE,__ATOMIC_SEQ_CST);
	printf("\n");

	__atomic_compare_exchange_n(pa,pb,4,0,__ATOMIC_ACQ_REL,__ATOMIC_RELAXED);
	__atomic_compare_exchange_n(pa,pb,4,0,__ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE);
	//__atomic_compare_exchange_n(pa,pb,4,0,__ATOMIC_ACQ_REL,__ATOMIC_RELEASE);
	//__atomic_compare_exchange_n(pa,pb,4,0,__ATOMIC_ACQ_REL,__ATOMIC_ACQ_REL);
	//__atomic_compare_exchange_n(pa,pb,4,0,__ATOMIC_ACQ_REL,__ATOMIC_SEQ_CST);
	printf("\n");

	__atomic_compare_exchange_n(pa,pb,4,0,__ATOMIC_SEQ_CST,__ATOMIC_RELAXED);
	__atomic_compare_exchange_n(pa,pb,4,0,__ATOMIC_SEQ_CST,__ATOMIC_ACQUIRE);
	//__atomic_compare_exchange_n(pa,pb,4,0,__ATOMIC_SEQ_CST,__ATOMIC_RELEASE);
	//__atomic_compare_exchange_n(pa,pb,4,0,__ATOMIC_SEQ_CST,__ATOMIC_ACQ_REL);
	__atomic_compare_exchange_n(pa,pb,4,0,__ATOMIC_SEQ_CST,__ATOMIC_SEQ_CST);
	printf("\n");

	/*
	ret = 0xff;
	ret = __sync_bool_compare_and_swap(pa,4,5);
	printf("now a:%d,ret:%d\n",a,ret);

	b=5;    //expect a==5, change to desire value 4
	ret = __atomic_compare_exchange_n(pa,pb,4,0,__ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE);
	printf("now a:%d,ret:%d\n",a,ret);

	b=4;    //expect a==4, change to desire value 3
	ret = __atomic_compare_exchange_n(pa,pb,3,0,__ATOMIC_RELEASE,__ATOMIC_ACQUIRE);
	printf("now a:%d,ret:%d\n",a,ret);

	b=4;    //expect a==4, but now a=3, will not change to desire value 2
	ret = __atomic_compare_exchange_n(pa,pb,2,0,__ATOMIC_CONSUME,__ATOMIC_ACQUIRE);
	printf("now a:%d,ret:%d\n",a,ret);
	/
	return 0;
}
