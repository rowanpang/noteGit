#include <stdio.h>

int main(int argc,char** argv)
{
	int n = 50;
	int i,j;
	float x=0.0;
	float sum = 0.0;
#if 1
	for(i=1;i<n;i++){
		x=1;
		for(j=1;j<i+1;j++){
			x=x*j;
		}
		sum += 1/x;
	}
#else
	x=1.0;
	for(i=1;i<n;i++){
		x=x*i;
		sum += 1/x;
	}
#endif	
	printf("%f\n",sum);	
	return 0;
}
