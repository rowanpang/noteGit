#include <stdio.h>

int main(int argc,char** argv)
{
	int map[4]={18,12,24,30}; 
	int gcd = 1;
	int i,j;
	
	for(i=2;i<=12;i++){
		for(j=0;j<4;j++){
			if((map[j]%i)!=0)
				break;
		}
		if(j==4)
			gcd=gcd>i?gcd:i;
	}

	printf("%d \n",gcd);
	return 0;
}


