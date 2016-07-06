#include<stdio.h>

struct my_st{
	int day;
	int mo;
	int arry[3];
};


#define GET_VAL()	\
	({				\
		int b_te = 10;	\
		b_te;			\
	})

void main (void)
{
	struct my_st st={.mo=8};
	int *pa,*pe,*pe_2;
	int a;

	//a=GET_VAL();

	a=({ int b_te = 10; b_te;});

	pa = &st.arry[0];
	pe = pa + 1;
	pe_2= pa + 2;
	pa = &(st.arry[2]);

	printf("addr test,pa:%ld,pe:%ld,pe_2:%ld,diff:%ld\n",pa,pe,pe_2,pe_2-pe);
	printf("addr a[0]:%ld,a[2]:%ld\n",&st.arry[0],&st.arry[2]);

}

