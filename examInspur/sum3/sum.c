#include <stdio.h>

int main(int argc,char** argv)
{
	int n = 10;
	int map[10]={1,2,3,4,5,6,7,8,9};
	int hint;
	int i,j,k;
	int sum = 0;
	for(i=0;i<n;i++){
		for(j=i;j<n;j++){
			sum = map[j] + sum;
			printf("j:%d,map[j]:%d,sum:%d\n",j,map[j],sum);
			if(sum == 3){
				printf("hint\n");
				hint++;
				break;
			}
		}
		sum = 0;
	}

	for(i=0;i++;);

	printf("%d\n",hint);
}
