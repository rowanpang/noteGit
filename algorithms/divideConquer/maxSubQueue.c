#include <stdio.h>
#include <stdlib.h>

int maxSubQueueSum(int *A,int n)
{
    int i,j,k;
    int thisSum,maxSum;
    for(i=0;i<n;i++){
	for(j=i;j<n;j++){
	    thisSum = 0;
	    maxSum = 0;

	    for(k=i;k<=j;k++){
		thisSum += A[k];
	    }

	    if(thisSum > maxSum){
		maxSum = thisSum;
	    }
	}
    }

    return maxSum;
}

int maxSubQueueSum2(int *A,int n)
{
    int i,j;
    int thisSum,maxSum;
    for(i=0;i<n;i++){
	thisSum = 0;
	maxSum = 0;
	for(j=i;j<n;j++){
	    thisSum += A[j];
	    if(thisSum > maxSum){
		maxSum = thisSum;
	    }
	}
    }

    return maxSum;
}

#define MAX3(a,b,c) (a>b?(a>c?a:c):(b>c?b:c))

int maxSubQueueSum3(int *A,int l,int r)
{
    int c,i;
    int leftSum,rightSum;
    int leftBorderSum,rightBorderSum;
    int leftMaxBorderSum,rightMaxBorderSum;
    c = l + (r-l)/2;

    if(l == r){
	return A[l]>0 ? A[l] : 0;
    }

    leftSum = maxSubQueueSum3(A,l,c);
    rightSum = maxSubQueueSum3(A,c+1,r);

    leftBorderSum = 0;leftMaxBorderSum= 0;
    for(i=c;i>=l;i--){
	leftBorderSum += A[i];
	if(leftBorderSum > leftMaxBorderSum){
	    leftMaxBorderSum = leftBorderSum;
	}
    }

    rightBorderSum = 0;rightMaxBorderSum= 0;
    for(i=c+1;i<=r;i++){
	rightBorderSum += A[i];
	if(rightBorderSum > rightMaxBorderSum){
	    rightMaxBorderSum = rightBorderSum;
	}
    }

    return MAX3(leftSum,rightSum,leftMaxBorderSum+rightMaxBorderSum);
}

int maxSubQueueSum4(int *A,int n)
{
    int thisSum,maxSum,j;
    thisSum = maxSum = 0;
    for(j=0;j<n;j++){
	thisSum += A[j];

	if(thisSum > maxSum){
	    maxSum = thisSum;
	}else if (thisSum < 0){
	    thisSum = 0;
	}
	/*因为题目定义,如果sum<0,则返回0. 所以,如果thisSum<0,表明可以重新计算sum*/
    }

    return maxSum;
}

int main(int argc,char **argv)
{
    /*int A[8] = {4,-3,5,-2,-1,2,6,-2};*/
    int A[8] = {-4,-3,-5,-2,-1,-2,-6,-2};
    int ret;

    int n = sizeof(A)/sizeof(A[0]);
    ret = maxSubQueueSum(A,n);
    printf("Θ(n^3) maxSum:%d\n",ret);

    ret = maxSubQueueSum2(A,n);
    printf("Θ(n^2) maxSum:%d\n",ret);

    ret = maxSubQueueSum3(A,0,n-1);
    printf("Θ(nlgn) maxSum:%d\n",ret);

    ret = maxSubQueueSum4(A,n);
    printf("Θ(n) maxSum:%d\n",ret);
}
