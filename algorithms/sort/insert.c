#include <stdio.h>

int sortUp(int *ary,int n)
{
    int j,i,tmp;

    for (j = 1; j < n; j++) {
	tmp = ary[j];
	i = j;

	while(i--){
	    if(ary[i] < tmp){
		break;
	    }
	    ary[i+1] = ary[i];
	}
	ary[i+1] = tmp;

	/*
	 *while(ary[i-1] > tmp && i){
	 *    ary[i] = ary[i-1];
	 *    i--;
	 *}
	 *ary[i] = tmp;
	 */
    }

    return 0;
}

int main(int argc,char **argv)
{
    int a1[10] = {5,8,3,2,4,1,9};
    sortUp(a1,sizeof(a1)/sizeof(int));

    for (int i = 0; i < sizeof(a1)/sizeof(int); ++i) {
	printf("%d ",a1[i]);
    }

    printf("\n");
}
