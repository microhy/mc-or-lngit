//#include<stdio.h>

#include"uart.h"

volatile unsigned long timestamp = 0;


#define   DRAM_MEM_SIZE       (0x200)
#define   DRAM_ADDR_START     (0x1FFF-DRAM_MEM_SIZE+1)
#define   DRAM_ADDR_END       (0x1FFF)

#define Struct_Section  __attribute__((unused, section(".stdata_mem_type"), \
		aligned(4)))

typedef unsigned int u32;
typedef signed int   s32;

 char txbuf[]="Hello or";
 char testbss;
 char testdata=10;
 typedef  struct {
	char ch[4];
	unsigned int addrinit;
	unsigned int len;
 }STDATA_MEM_TYPE;

STDATA_MEM_TYPE arry_dynamic Struct_Section;


int main(void)
{
    int i=0;
	u32 *ptr_ram;

	ptr_ram =(u32 *)DRAM_ADDR_START;

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

	*ptr_ram++ = 0xffff0000;

	arry_dynamic.ch[0]=8%10;
	arry_dynamic.addrinit=(u32)&i; 
	arry_dynamic.len=1;
 
	*ptr_ram++ = arry_dynamic.ch[0];
	*ptr_ram++ = arry_dynamic.addrinit;
	*ptr_ram++ = arry_dynamic.len;
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
