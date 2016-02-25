#include<stdio.h>
#include<string.h>

struct utf8_table {
	int     cmask;
	int     cval;
	int     shift;
	long    lmask;
	long    lval;
};

static const struct utf8_table utf8_table[] =
{
   //cmask  cval   shift   lmask		   lval
    {0x80,  0x00,   0*6,    0x7F,           0,         /* 1 byte sequence */},
    {0xE0,  0xC0,   1*6,    0x7FF,          0x80,      /* 2 byte sequence */},
    {0xF0,  0xE0,   2*6,    0xFFFF,         0x800,     /* 3 byte sequence */},
    {0xF8,  0xF0,   3*6,    0x1FFFFF,       0x10000,   /* 4 byte sequence */},
    {0xFC,  0xF8,   4*6,    0x3FFFFFF,      0x200000,  /* 5 byte sequence */},
    {0xFE,  0xFC,   5*6,    0x7FFFFFFF,     0x4000000, /* 6 byte sequence */},
    {0,						       /* end of table    */}
};

#define UNICODE_MAX	0x0010ffff
#define PLANE_SIZE	0x00010000

#define SURROGATE_MASK	0xfffff800
#define SURROGATE_PAIR	0x0000d800
#define SURROGATE_LOW	0x00000400
#define SURROGATE_BITS	0x000003ff

typedef unsigned long unicode_t;
typedef unsigned char u8;
typedef unsigned long u32;




int utf8_to_utf32(const u8 *s, int len, unicode_t *pu)
{
	unsigned long l;
	int c0, c, nc;
	const struct utf8_table *t;
  
	nc = 0;
	c0 = *s;
	l = c0;
	printf("c0:0x%x,l:0x%x\n",c0,l);
	for (t = utf8_table; t->cmask; t++) {
		nc++;
		if ((c0 & t->cmask) == t->cval) {
			l &= t->lmask;
			printf("value l:0x%x\n",l);
			if (l < t->lval || l > UNICODE_MAX ||
					(l & SURROGATE_MASK) == SURROGATE_PAIR)
				return -1;
			*pu = (unicode_t) l;
			printf("before return,pu 0x:%x\n",*pu);
			return nc;
		}
		if (len <= nc)
			return -1;
		printf("s:0x%x\n",*s);
		s++;
		c = (*s ^ 0x80) & 0xFF;
		if (c & 0xC0)
			return -1;
		l = (l << 6) | c;
	}
	return -1;
}


int utf32_to_utf8(unicode_t u, u8 *s, int maxlen)
{
	unsigned long l;
	int c, nc;
	const struct utf8_table *t;

	if (!s)
		return 0;

	l = u;
	if (l > UNICODE_MAX || (l & SURROGATE_MASK) == SURROGATE_PAIR)
		return -1;

	nc = 0;
	for (t = utf8_table; t->cmask && maxlen; t++, maxlen--) {
		nc++;
		if (l <= t->lmask) {
			c = t->shift;
			*s = (u8) (t->cval | (l >> c));
			while (c > 0) {
				c -= 6;
				s++;
				*s = (u8) (0x80 | ((l >> c) & 0x3F));
			}
			return nc;
		}
	}
	return -1;
} 

int main(void)
{
	//unsigned char in[]={0xe2,0x96,0x88};
	unsigned char in[]={"abc庞伟振"};
	u32 out[10]={0};	
	u32 o = 0;

	u32 size = sizeof(in);
	printf("strlen:%d\n",size);
	int result=0;	
	u8 step = 1;
	const struct utf8_table *t;
	u8 conver_p=0;
	while( o < size )
	{
		printf("\n\nnew circle ");
		printf("cur position:%d\n",o);
		step = 1;
		for (t = utf8_table; t->cmask; t++)
		{
			printf("in[%d]:0x%x\n",o,in[o]);
			if((in[o] & t->cmask) == t->cval)
			{
				break;
			}	
			step++;
		}		
		printf("setup=%d\n",step);	
		result = utf8_to_utf32(&in[o],step,&out[conver_p]);

		printf("sizeof :%d\n",sizeof(u32));
		printf("unicode: %#0*x\n",2+sizeof(u32),out[conver_p]);
		conver_p++;
		o += step;
		
	}	

	u8 outtmp[10]={0};
	utf32_to_utf8(0x5e9e,&outtmp[0],10);

	printf("utf8out:0x%x 0x%x 0x%x\n",outtmp[0],outtmp[1],outtmp[2]);
	
	//int utf32_to_utf8(unicode_t u, u8 *s, int maxlen)
	//printf("result %d\n",result);
	//printf("test 0x%x\n",out);
	//printf("0x format. utf8: %x %x %x convert ot utf32 is[MSB first]:",in[0],in[1],in[2]);
	//printf("%x %x %x %x \n",(out&0xff000000)>>24,(out&0x00ff0000)>>16,(out&0x0000ff00)>>8,(out&0x000000ff)>>0);

	unsigned char in_gbk[]={0xc5,0xd3};
	unsigned char out_gbk[10]={0xc5,0xd3};
	result = utf8_to_utf32(&in_gbk[0],2,&out[0]);
	printf("result %d\n",result);
	printf("0x format. utf8: %x %x %x convert ot utf32 is[MSB first]:\n",out_gbk[0],out_gbk[1],out_gbk[2]);
}

