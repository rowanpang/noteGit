#include <stdio.h>
#include <stdlib.h>

struct PointPeople{
    int n;
    int m;
    int nuPeople;
};
struct PointPeople *PP = NULL;

#ifndef false
#define false 0
#define true 1
#endif

int xMax=4,yMax=5;
int nuPointsPeople=0;

int pointPeopleNum(int n,int m)
{
    int k;
    for(k=0;k<nuPointsPeople;k++) {
        if((PP[k].n == n) && (PP[k].m == m)){
            return PP[k].nuPeople;
	}
    }

    return 0;
}
int maxPeople = 0;

int testMove(int n,int m,int people)
{
    printf("%d,%d \n",n,m);
    people+=pointPeopleNum(n,m);
    if(people > maxPeople){
        maxPeople = people;
    }

    if(n == xMax && m == yMax){
	printf("people:%d\n",people);
    }


    if(n<xMax) {
        /*printf("move right\n");*/
        testMove(n+1,m,people);
    }

    if(m<yMax) {
        /*printf("move up\n");*/
        testMove(n,m+1,people);
    }

    return 0;
}

int main(int argc,char** argv)
{
    int ntmp=1,mtmp=1;
    freopen("in.txt","r",stdin);
    scanf("%d %d %d",&xMax,&yMax,&nuPointsPeople);
    PP = malloc(sizeof(struct PointPeople)*nuPointsPeople);
    int k=0;
    for(k=0;k<nuPointsPeople;k++) {
        scanf("%d %d %d",&PP[k].n,&PP[k].m,&PP[k].nuPeople);
        //printf("k:%d,n:%d,m:%d,people:%d\n",k,PP[k].n,PP[k].m,PP[k].nuPeople);
    }

    testMove(ntmp,mtmp,0);

    printf("maxPeople:%d\n",maxPeople);
    freopen("out.txt","w",stdout);
    printf("%d\n",maxPeople);

    return 0;
}
