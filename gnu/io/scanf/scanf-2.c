#include<stdio.h>
int main(int argc,char** argv)
{
    int i;
    float v1,v2,v3,v4,v5,i1,i2,i3,i4,i5;
    int num = 1;
    for(i=1;i<=num;i++)
    {
	scanf("%f %f %f %f %f",&i1,&i2,&i3,&i4,&i5);
	switch(i){
	    case 1 : v1=(i1+i2+i3+i4+i5)/5;break;
	    case 2 : v2=(i1+i2+i3+i4+i5)/5;break;
	    case 3 : v3=(i1+i2+i3+i4+i5)/5;break;
	    case 4 : v4=(i1+i2+i3+i4+i5)/5;break;
	    case 5 : v5=(i1+i2+i3+i4+i5)/5;break;
	    default:;
	}
    }

    for(i=1;i<=num;i++)
    {
	switch(i){
	    case 1 : printf("\nNO. V1=%f",v1);break;
	    case 2 : printf("\nNO. V2=%f",v2);break;
	    case 3 : printf("\nNO. V3=%f",v3);break;
	    case 4 : printf("\nNO. V4=%f",v4);break;
	    case 5 : printf("\nNO. V5=%f",v5);break;
	    default:;
	}
    }    
    printf("\n");
    return 0;
}
