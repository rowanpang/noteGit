#include<stdio.h>

//pangwz
//    if arg... is none,'##'remove the pre ',',so dprintf("---\n") is ok!!! and '..,st' is also ok!!!
#define dprintf(fmt,arg...) printf("pangwz"fmt,##arg)
//pangwz,
//    after spread out there is a ',' so need one more arg,just dprintf("---\n") is error!!!.need dprintf("---\n",st);
//#define dprintf(fmt,arg...) printf("pangwz "fmt,arg)
//#define dprintf(fmt,...) printf("pangwz "fmt,__VA_ARGS__)

typedef enum hostStatus {
    linked=0,
    agentRunning
} hostStatus_t;

#define stStr(st) #st

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

    printf("sizeof(long):%d\n",sizeof(long));
    printf("sizeof(unsigned long):%d\n",sizeof(unsigned long));
    printf("sizeof(unsigned long int):%d\n",sizeof(unsigned long int));
    printf("sizeof(unsigned short int):%d\n",sizeof(unsigned short int));
    printf("sizeof(unsigned int):%d\n",sizeof(unsigned int));
    printf("sizeof(void*):%d\n",sizeof(void*));

    st_2 = st_2;
    st = st;

    union u_1{
        unsigned int union_32;

        struct u_s_1{
            unsigned char a;
            unsigned char b;
            unsigned char c;
            unsigned char d;
            unsigned char e;
            unsigned char f;
        }union_sp_8;

        struct u_s_2{
            unsigned short int a;
            unsigned short int b;
            unsigned short int c;
        }union_sp_16;
    }u;

    u.union_32 = 0x12345678;
    u.union_sp_16.c = 0x9abc;
    printf("sizeof(union u):%d\n",sizeof(u));
    printf("sizeof(union u.union_sp_8):%d\n",sizeof(u.union_sp_8));
    printf("sizeof(union u.union_sp_16):%d\n",sizeof(u.union_sp_16));
    printf("u.union_sp_8:%p\n",&u.union_sp_8);
    printf("u.union_sp_16:%p\n",&u.union_sp_16);

    printf("u.union_32:%p,%#x\n",&u.union_32,u.union_32);
    printf("u.union_sp_8.a:%p,%#x\n",&u.union_sp_8.a,u.union_sp_8.a);
    printf("u.union_sp_8.b:%p,%#x\n",&u.union_sp_8.b,u.union_sp_8.b);
    printf("u.union_sp_8.c:%p,%#x\n",&u.union_sp_8.c,u.union_sp_8.c);
    printf("u.union_sp_8.d:%p,%#x\n",&u.union_sp_8.d,u.union_sp_8.d);
    printf("u.union_sp_8.e:%p,%#x\n",&u.union_sp_8.e,u.union_sp_8.e);
    printf("u.union_sp_8.f:%p,%#x\n",&u.union_sp_8.f,u.union_sp_8.f);
    printf("u.union_sp_16.a:%p,%#x\n",&u.union_sp_16.a,u.union_sp_16.a);
    printf("u.union_sp_16.b:%p,%#x\n",&u.union_sp_16.b,u.union_sp_16.b);
    printf("u.union_sp_16.c:%p,%#x\n",&u.union_sp_16.c,u.union_sp_16.c);


    char* pArray[]={ "a","b"};
    printf("sizeof(pArray):%d\n",sizeof(pArray)/sizeof(char*));
    
    int a = 2;
    _Bool testBool=3;
    _Bool testBool2=a;
    printf("sizeof(testBool):%d,value:%d\n",sizeof(testBool),testBool);
    printf("sizeof(testBool2):%d,value:%d\n",sizeof(testBool2),testBool2);

    hostStatus_t hst=agentRunning;
    printf("enum type status:%s,%d\n",stStr(agentRunning),agentRunning);


    return 0;
}

