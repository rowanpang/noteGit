#include <stdio.h>

#if 1
	#define dprintf(fmt,arg...) printf(fmt,##arg)
#else
	#define dprintf(fmt,arg...)
#endif

int main(int argc,char** argv)
{
	int map[50][50];
	int i,j;
	int numPrint=2;
	char growDown = 1;//init down direction growup
	char target = 20
	i=1;
	j=1;
	map[0][0]=1;
	int n;
	while(1){
		dprintf("------------\n");
		if(growDown){
			i=0;
			dprintf("down\n");
			for(j;j>=0;j--,i++){
				map[i][j]=numPrint++;
				if(numPrint==(target+1)){
					dprintf("i:%d,j:%d,num:%d\n",i,j,map[i][j]);
				}
			}
		}else{
			dprintf("up\n");
			j=0;	
			for(i;i>=0;i--,j++){
				map[i][j]=numPrint++;
				if(numPrint==(target+1)){
					dprintf("i:%d,j:%d,num:%d\n",i,j,map[i][j]);
				}
			}
		}

		growDown=!growDown;

		if(numPrint>target){
			n = i>j?i:j;
			break;
		}
	}
	
	for(i=0;i<n;i++){
		for(j=0;j<n-i;j++){
			printf("%3d ",map[i][j]);
		}
		printf("\n");
	}
}
