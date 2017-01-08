//#include<stdio.h>

#include"uart.h"

volatile unsigned long timestamp = 0;


#define   RAM_MEM_SIZE       (0x400)
#define   RAM_ADDR_START     (0x1FFF-RAM_MEM_SIZE+1)
#define   RAM_ADDR_END       (0x1FFF)

typedef unsigned int u32;
typedef signed int   s32;

int main(void)
{
    char txbuf[]="Hello or";
    int i=0;
	u32 *ptr_ram;

	ptr_ram =(u32 *)RAM_ADDR_START;

    for(i=0; i<30; ++i)
	{
		*ptr_ram = 0x66660000;
		ptr_ram++;
	}
    
	uart_init();
	for(i=0;i<7; i++) 
	{
		uart_putc(txbuf[i]);
	}

	*ptr_ram = 0xffff0000;
    while(1);
    return 0;
}

void int_error(void)
{

    while(1);
}

void int_main(void)
{
    while(1);
}
