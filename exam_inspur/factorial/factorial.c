#include <stdio.h>

int fac(int i,int j)
{
	i=i*j;	
	j++;
	if(j>4){
		printf("val:%d\n",i);
		return i;
	}
	fac(i,j);
	return i;
}




int main(int argc,char** argv)
{

	fac(1,1);
	return 0;
}
