//#include "support.h"
//#include<stdio.h>
#include "board.h"
#include "uart.h"

#define BOTH_EMPTY (UART_LSR_TEMT | UART_LSR_THRE)

#define WAIT_FOR_XMITR \
        do { \
                lsr = REG8(UART_BASE + UART_LSR); \
        } while ((lsr & BOTH_EMPTY) != BOTH_EMPTY)

#define WAIT_FOR_THRE \
        do { \
                lsr = REG8(UART_BASE + UART_LSR); \
        } while ((lsr & UART_LSR_THRE) != UART_LSR_THRE)

#define CHECK_FOR_CHAR (REG8(UART_BASE + UART_LSR) & UART_LSR_DR)

#define WAIT_FOR_CHAR \
         do { \
                lsr = REG8(UART_BASE + UART_LSR); \
         } while ((lsr & UART_LSR_DR) != UART_LSR_DR)

#define UART_TX_BUFF_LEN 32
#define UART_TX_BUFF_MASK (UART_TX_BUFF_LEN -1)

char tx_buff[UART_TX_BUFF_LEN];
volatile int tx_level, rx_level;

void uart_init(void)
{
        int divisor;
    float float_divisor;

        /* Reset receiver and transmiter */
        REG8(UART_BASE + UART_FCR) = UART_FCR_ENABLE_FIFO | UART_FCR_CLEAR_RCVR
        | UART_FCR_CLEAR_XMIT | UART_FCR_TRIGGER_4;
 
        /* Disable all interrupts */
        REG8(UART_BASE + UART_IER) = 0x00;
 
        /* Set 8 bit char, 1 stop bit, no parity */
        REG8(UART_BASE + UART_LCR) = UART_LCR_WLEN8 & ~(UART_LCR_STOP | 
                            UART_LCR_PARITY);

        /* Set baud rate */
//    float_divisor = IN_CLK/(16.0 * UART_BAUD_RATE);
//    float_divisor += 0.50f; // Ensure round up
        //divisor = (int) float_divisor;
        divisor = 28;

        REG8(UART_BASE + UART_LCR) |= UART_LCR_DLAB;
        REG8(UART_BASE + UART_DLL) = divisor & 0x000000ff;
        REG8(UART_BASE + UART_DLM) = (divisor >> 8) & 0x000000ff;
        REG8(UART_BASE + UART_LCR) &= ~(UART_LCR_DLAB);
}

void uart_putc(char c)
{
        unsigned char lsr;
        
        WAIT_FOR_THRE;
        REG8(UART_BASE + UART_TX) = c;
        if(c == '\n') {
          WAIT_FOR_THRE;
          REG8(UART_BASE + UART_TX) = '\r';
        }
        WAIT_FOR_XMITR;
}

char uart_getc(void)
{
        unsigned char lsr;
        char c;

        WAIT_FOR_CHAR;
        c = REG8(UART_BASE + UART_RX);
        return c;
}

//hy add 20170322
void uart_print_str(const char *str)
{
        unsigned char lsr;
        
		while(*str!='\0')
		{		
        	WAIT_FOR_THRE;
        	REG8(UART_BASE + UART_TX) = *str++;
	        if(*str == '\n') {
    	      WAIT_FOR_THRE;
        	  REG8(UART_BASE + UART_TX) = '\r';
	        }
		}
        WAIT_FOR_XMITR;        
}

//hy add 2017-2-25
void uart_put_num(unsigned int num)
{
    unsigned char lsr;
    char c[10]={'0','0','0','0','0','0','0','0','0','0'};
    char *p=&c[9];
   
	if(!num)
	{	
		p--;
	}
	else
	{	
    	while(num!=0)
	    {
        	*p= (num%10)+'0';
			p--;    
		    num=num/10;
    	} 
	} 
    // p point the front of late valid c for the num
    for(p++; p<c+10; p++)
    {
        WAIT_FOR_THRE;
        REG8(UART_BASE + UART_TX) = *p;
        if(*p == '\n') {
          WAIT_FOR_THRE;
          REG8(UART_BASE + UART_TX) = '\r';
        }
        WAIT_FOR_XMITR;        
    }
}
/*
char uart_testc(void)
{
        if((REG8(UART_BASE + UART_LSR) & UART_LSR_DR) == UART_LSR_DR)
                return REG8(UART_BASE + UART_RX);
        else
                return 0;
}*/
