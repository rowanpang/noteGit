#include<stdio.h>

//pangwz
//	if arg... is none,'##'remove the pre ',',so dprintf("---\n") is ok!!! and '..,st' is also ok!!!
#define dprintf(fmt,arg...) printf("pangwz"fmt,##arg)
//pangwz,
//	after spread out there is a ',' so need one more arg,just dprintf("---\n") is error!!!.need dprintf("---\n",st);
//#define dprintf(fmt,arg...) printf("pangwz "fmt,arg)
//#define dprintf(fmt,...) printf("pangwz "fmt,__VA_ARGS__)
int main(int argc,char** argv)
{
	struct st{
		char* str1;
		char* str2;
		char* str3;
	}st={
		"pangwz",
		"rowan",
		"inspur"
		};
	
	struct st_2{
		char str1[10];
		char str2[10];
		char str3[10];
	}st_2={
		"pangwz",
		"rowan",
		"inspur"
		};

	dprintf("---\n");
	dprintf("---\n",st);
	dprintf("---%s\n","macro test");
	printf("sizeof(unsigned long):%d\n",sizeof(unsigned long));
	printf("sizeof(unsigned long int):%d\n",sizeof(unsigned long int));
	printf("sizeof(unsigned short int):%d\n",sizeof(unsigned short int));
	printf("sizeof(unsigned int):%d\n",sizeof(unsigned int));
	st_2 = st_2;
	st = st;

	union u_1{
		struct u_s_1{
			unsigned char a;
			unsigned char b;
			unsigned char c;
			unsigned char d;
			unsigned char e;
			unsigned char f;
		}union_sp_8;

		unsigned int union_32;

		struct u_s_2{
			unsigned short int a;
			unsigned short int b;
			unsigned short int c;
		}union_sp_16;
	}u;

	u.union_32 = 0x12345678;
	u.union_sp_16.c = 0x9abc;
	printf("sizeof(union u):%d\n",sizeof(u));
	printf("u.union_32:%#x\n",u.union_32);
	printf("u.union_sp_8.a:%#x\n",u.union_sp_8.a);
	printf("u.union_sp_8.b:%#x\n",u.union_sp_8.b);
	printf("u.union_sp_8.c:%#x\n",u.union_sp_8.c);
	printf("u.union_sp_8.d:%#x\n",u.union_sp_8.d);
	printf("u.union_sp_8.e:%#x\n",u.union_sp_8.e);
	printf("u.union_sp_8.f:%#x\n",u.union_sp_8.f);
	printf("u.union_sp_16.a:%#x\n",u.union_sp_16.a);
	printf("u.union_sp_16.b:%#x\n",u.union_sp_16.b);
	printf("u.union_sp_16.c:%#x\n",u.union_sp_16.c);

	return 0;
}

