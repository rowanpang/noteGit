#include <stdio.h>
#include <stdlib.h>

typedef unsigned int UINT;

struct PointPeople{
	UINT n;
	UINT m;
	UINT nuPeople;
};

struct PointPeople *PP = NULL;

#ifndef false
#define false 0
#define true 1
#endif

UINT nTotal=4,mTotal=5;
UINT nuPointsPeople=0;

UINT pointPeopleNum(UINT n,UINT m)
{
	UINT k;
	for(k=0;k<nuPointsPeople;k++)
	{
		if((PP[k].n == n) && (PP[k].m == m))
			return PP[k].nuPeople;
	}
	return 0;
}
UINT maxPeople = 0;
UINT testMove(UINT n,UINT m,UINT people)
{
	printf("n:%d,m:%d\n",n,m);
	people+=pointPeopleNum(n,m);
	if(people > maxPeople)
		maxPeople = people;	

	if(n<nTotal)
	{
		printf("move right\n");
		testMove(n+1,m,people);
	}
	if(m<mTotal)
	{
		printf("move up\n");
		testMove(n,m+1,people);	
	}

	printf("move out\n");
}

UINT main(UINT argc,char** argv)
{
	UINT ntmp=1,mtmp=1;
	freopen("bus.in","r",stdin);
	scanf("%d %d %d",&nTotal,&mTotal,&nuPointsPeople);
	PP = malloc(sizeof(struct PointPeople)*nuPointsPeople);
	UINT k=0;
	for(k=0;k<nuPointsPeople;k++)
	{
		scanf("%d %d %d",&PP[k].n,&PP[k].m,&PP[k].nuPeople);
		//printf("k:%d,n:%d,m:%d,people:%d\n",k,PP[k].n,PP[k].m,PP[k].nuPeople);
	}
	testMove(ntmp,mtmp,0);

	printf("maxPeople:%d\n",maxPeople);
	freopen("bus.out","w",stdout);
	printf("%d\n",maxPeople);

	return 0;
}
