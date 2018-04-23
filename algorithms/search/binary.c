#include <stdio.h>
#include <stdlib.h>

/*find x in a[l,r]*/
int binaryFind(int x,int *ary,int l,int r)
{
    int tmpI = l+(r-l)/2;
    int tmp = ary[tmpI];

    if((l == r) && (ary[l] != x)){
	return -1; /*not find*/
    }

    if( x == tmp){
	return tmpI;
    }else if(x < tmp){
	r = tmpI-1;
    }else{
	l = tmpI+1;
    }

    return binaryFind(x,ary,l,r);
}

int binarySteps(int x,int *ary,int l,int r)
{
    int tmpI = l+(r-l)/2;
    int tmp = ary[tmpI];

    if((l == r) && (ary[l] != x)){
	return 1; /*not find*/
    }

    if( x == tmp){
	return 1;
    }else if(x < tmp){
	r = tmpI-1;
    }else{
	l = tmpI+1;
    }

    return 1+binarySteps(x,ary,l,r);
}

int main(int argc,char **argv)
{
    int sortedArray[10] = {0,1,2,3,4,5,6,7,8,9};
    int nums = sizeof(sortedArray)/sizeof(sortedArray[0]);

    int n = -1;

    scanf("%d",&n);
    printf("n:%d\n",n);

    printf("find:%d with index:%d by steps:%d\n",
	    n,binaryFind(n,sortedArray,0,nums-1),binarySteps(n,sortedArray,0,nums-1));

}
