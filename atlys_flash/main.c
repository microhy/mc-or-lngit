#include <stdio.h>

//#define   DBG_OUT_FILE  
//#define   DP_EN
#define   UART_EN
#define   STDATA_EN

#ifdef UART_EN
    #include"board.h"
    #include"uart.h"
    #define  WRT_GPIO(addr,val)  (*(unsigned char *)(addr)=(val))
#endif

#ifdef   DP_EN
    #include "pixel_left.h"
    #include "pixel_right.h"
#endif

volatile unsigned long timestamp = 0;
#ifdef STDATA_EN
	#define Struct_Section __attribute__((unused, section(".stdata_mem_type"),aligned(4)))

	typedef  struct {
		char ch[4];
		unsigned int addrinit;
		unsigned int len;
	}STDATA_MEM_TYPE;
	STDATA_MEM_TYPE arry_dynamic Struct_Section;
#endif


#define   M   (100)

unsigned int g_pixel[M][M]={{0},{0}};


void board_init()
{
	uart_init();
	uart_putc('y');
	uart_putc('\n');
	uart_put_num(123455);
	uart_putc('\n');
	uart_print_str("----board init ok!\n");
}

void int_error()
{
	uart_print_str("<isr> int_error\n");
	while(1);
}

void int_main()
{
	uart_print_str("<isr> int_main\n");
	while(1);
}

int main()
{
	int i,j;
	unsigned int b_num[M];

	board_init();
	
	for(i=0; i<M; i++)
	{
		b_num[i] = i;
		for(j=0; j<M; j++)
		{
			g_pixel[i][j]=i*j;
		}
	}
	for(i=0; i<M; i++)
	{
		for(j=0; j<M; j++)
		{
			uart_put_num(g_pixel[i][j]);
			uart_putc('\t');
		}
		uart_putc('\n');
	}

	uart_print_str("----main exit\n");
	return 0;
}
