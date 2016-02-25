#include <stdio.h>

#define DEBUG_LEVEL 1
#if(DEBUG_LEVEL==2)
	#define dprintf(fmt,arg...) printf(fmt,##arg)
	#define ddprintf(fmt,arg...) printf(fmt,##arg)
#elif(DEBUG_LEVEL==1)
	#define dprintf(fmt,arg...) printf(fmt,##arg)
	#define ddprintf(fmt,arg...)
#else
	#define dprintf(fmt,arg...)
	#define ddprintf(fmt,arg...)
#endif

int main(int argc,char** argv)
{
	int nMeets=0;
	int nMeetsArranged=0;
	int begin[10000]={-1};	
	int end[10000]={-1};	
	int roomEnd[10000]={-1};	
	int nRooms;	
	int arranged[10000/32+1]={0};
	int earlyTime=0x7fffffff;

#define MEET_TO_INDEX(x) ((x) >> 5)
#define MEET_TO_MASK(x) (1 << ((x) & 31))
#define meet_raise(x) (arranged[MEET_TO_INDEX(x)] |= MEET_TO_MASK(x))
#define meet_lower(x) (arranged[MEET_TO_INDEX(x)] &= ~MEET_TO_MASK(x))
#define meet_raised(x)(arranged[MEET_TO_INDEX(x)] & MEET_TO_MASK(x))

	freopen("meet.in","r",stdin);
	scanf("%d",&nMeets);
	dprintf("%d\n",nMeets);

	int i,j;
	for(i=0;i<nMeets;i++){
		scanf("%d %d",&begin[i],&end[i]);
		dprintf("%d %d\n",begin[i],end[i]);
	}	
//pangwz: sort first
	int tmp;
	for(i=0;i<nMeets;i++){
		for(j=i;j<nMeets;j++){
			if(begin[i]>begin[j]){
				tmp = begin[i];
				begin[i] = begin[j];
				begin[j] = tmp;
	
				tmp = end[i];
				end[i] = end[j];
				end[j] = tmp;
			}
		}
	}

	dprintf("after sort--------\n");
	for(i=0;i<nMeets;i++){
		dprintf("%d %d\n",begin[i],end[i]);
	}

newCycle:	
	ddprintf("---newCycle----------\n");
	for(i=0;i<nMeets;i++){
		ddprintf("---inCycle-----deal meets:%d,ranged:%s\n",i,meet_raised(i)?"Yes":"No");
		if(!meet_raised(i)){
			for(j=0;j<nRooms;j++){
				ddprintf("---inCycle-----test room:%d,now roomend:%d---\n",j,roomEnd[j]);
				if(begin[i]>=roomEnd[j]){
					ddprintf("---inCycle-----hint room:%d,now roomend:%d---\n",j,roomEnd[j]);
					break;
				}
			}
			if(j==nRooms){
				nRooms++;
				ddprintf("---inCycle-----new room:%d,now roomend:%d---\n",j,roomEnd[j]);
			}
			roomEnd[j]=end[i];
			meet_raise(i);
			dprintf("--range meet:%d,room:%d,end:%d\n",i,j,end[i]);
			goto newCycle;
		}
	}
#if 1
	for(i=0;i<nMeets;i++){
		if(meet_raised(i))
			ddprintf("meet:%d,raised\n",i);
		else
			ddprintf("meet:%d,not raised\n",i);
	}
#endif

	printf("rooms:%d\n",(nRooms));


	return 0;
}
