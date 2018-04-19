#include <stdio.h>

int sortUp(int *ary,int n)
{
    int j,i,tmp;

    for (j = 1; j < n; j++) {
	tmp = ary[j];

	/*
	 *i = j;
	 *Not stability for order changed!!! ref
	 *    https://www.geeksforgeeks.org/stability-in-sorting-algorithms/
	 *while(i--){
	 *    if(ary[i] < tmp){
	 *        break;
	 *    }
	 *    ary[i+1] = ary[i];
	 *}
	 *ary[i+1] = tmp;
	 */

	i = j-1;
	while(i>=0 && ary[i] > tmp){
	    ary[i+1] = ary[i];
	    i--;
	}
	ary[i+1] = tmp;
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
