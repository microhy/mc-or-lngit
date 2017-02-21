#include"board.h"

#define  WRT_GPIO(addr,val)  (*(unsigned char *)(addr)=(val))

void delay_ms(int m)
{
	int i,j;

	for(i=0; i<m; ++i)
	{
		for(j=0; j<1000000/20; ++j);
	}

}



int main(void)
{
	unsigned char ch,i;
	char *version="17-02-18 v0.0";	


	WRT_GPIO(GPIO_0_BASE+0x3,0xff);
	WRT_GPIO(GPIO_0_BASE+0x4,0xff);
	WRT_GPIO(GPIO_0_BASE+0x5,0xff);

	WRT_GPIO(GPIO_0_BASE+0x0,0xaa);
	WRT_GPIO(GPIO_0_BASE+0x1,0xbb);
	WRT_GPIO(GPIO_0_BASE+0x2,0xcc);

	while(1)
	{
		for(i=0; i<8; ++i)
		{
			ch = 1<<i;
			WRT_GPIO(GPIO_0_BASE+0x0,ch);
			delay_ms(100);
		}
	}

    return 0;
}
