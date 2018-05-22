#include <stdio.h>

#if 1
	#define dprintf(fmt,arg...) printf(fmt,##arg)
#else
	#define dprintf(fmt,arg...) 
#endif

int main(int argc,char** argv)
{
	int n;
		
	freopen("change.in","r",stdin);
	scanf("%d",&n);
	dprintf("%d\n",n);
	int map[100];
	int i;
	for(i=0;i<n;i++){
		scanf("%d",&map[i]);
		dprintf("%d ",map[i]);
	}
	dprintf("\n");

	int tmp;
	int maxIndex,max;
	int cycle = 0;
	while(1){
		if(map[0] == 1)
		{
			for(i=0;i<n;i++){
				if(map[i] != (i+1))
					break;
			}
			dprintf("i:%d\n",i);
			if(i!=n){
				tmp = map[i];
				map[i] = map[0];
				map[0] = tmp;
				cycle++;
				for(i=0;i<n;i++){
					dprintf("%d ",map[i]);
				}
				dprintf("-------------\n");
			}else{
				dprintf("ok,just exit\n");
				break;
			}
		}

		tmp = map[0];
		map[0] = map[tmp-1];
		map[tmp-1] = tmp;
		cycle++;	

		for(i=0;i<n;i++){
			dprintf("%d ",map[i]);
		}
		dprintf("xxxxxxxxxxxx\n");
	}
	printf("cycle:%d\n",cycle);

	return 0;
}
