#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

typedef unsigned int UINT;

struct PointPeople{
    UINT n;
    UINT m;
    UINT nuPeople;
};
struct PointPeople *PP = NULL;

struct roution{
    int x;
    int y;
};


#ifndef false
#define false 0
#define true 1
#endif

UINT xMax=4,yMax=5;
UINT nuPointsPeople=0;
UINT maxPeople = 0;
int totalRoution=0;

UINT pointPeopleNum(UINT n,UINT m)
{
    UINT k;
    for(k=0;k<nuPointsPeople;k++) {
        if((PP[k].n == n) && (PP[k].m == m)){
            return PP[k].nuPeople;
	}
    }
    return 0;
}

UINT testMove(UINT n,UINT m,UINT people,struct roution **rinfo)
{
    int i,x,y;
    /*printf("n:%d,m:%d\n",n,m);*/
    (*rinfo)->x = n;
    (*rinfo)->y = m;
    (*rinfo)++;

    people+=pointPeopleNum(n,m);
    if(people > maxPeople){
        maxPeople = people;
    }

    if(n == xMax && m== yMax){
	i = 0;
	while(i < (n+m-1)){
	    x = ((*rinfo)-xMax-yMax+1+i)->x;
	    y = ((*rinfo)-xMax-yMax+1+i)->y;
	    printf("%d,%d:%d ",x,y,pointPeopleNum(x,y));
	    i++;
	}
	printf(" p:%d\n",people);

	totalRoution++;
    }

    if(n<xMax) {
        /*printf("move right\n");*/
        testMove(n+1,m,people,rinfo);
    }
    if(m<yMax) {
        /*printf("move up\n");*/
        testMove(n,m+1,people,rinfo);
    }

    (*rinfo)--;
    /*printf("move out\n");*/
}

UINT main(UINT argc,char** argv)
{
    UINT ntmp=1,mtmp=1;
    UINT k=0;
    struct roution *rinfo,*rtmp;

    PP = malloc(sizeof(struct PointPeople)*nuPointsPeople);
    memset(PP,0,sizeof(struct PointPeople)*nuPointsPeople);
    rinfo = malloc(sizeof(struct roution)*(xMax+yMax-1));
    memset(rinfo,0,sizeof(struct roution) * (xMax+yMax-1));
    rtmp = rinfo;

    freopen("bus.in","r",stdin);
    scanf("%d %d %d",&xMax,&yMax,&nuPointsPeople);
    for(k=0;k<nuPointsPeople;k++) {
        scanf("%d %d %d",&PP[k].n,&PP[k].m,&PP[k].nuPeople);
	printf("k:%d %d,%d:%d\n",k,PP[k].n,PP[k].m,PP[k].nuPeople);
    }
    testMove(ntmp,mtmp,0,&rtmp);

    printf("maxPeople:%d\n",maxPeople);
    printf("totalRoution:%d\n",totalRoution);
    freopen("bus.out","w",stdout);
    printf("%d\n",maxPeople);

    return 0;
}
