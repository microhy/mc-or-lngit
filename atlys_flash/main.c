#include <stdio.h>

//#define   DBG_OUT_FILE  
#define   DP_EN
//#define   UART_EN
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

#define   H        (480)
#define   W        (640)
#define   OR_NUM   (30)        //60->30 
#define   disparityRange     (20)
#define   halfBlockSize      (3)
#define   halfBlockSize2     (3)
#define   MAX(a,b) ((a)>(b))?(a):(b)
#define   MIN(a,b) ((a)<(b))?(a):(b)
#define   CFINF    (1000000000)

unsigned short int cvL[8][33][640] = {{0,0,0}};
unsigned short int cvR[8][33][640] = {{0,0,0}};
int disparityCost[W][20];
int optimalIndices[W][20];
unsigned char Ddynamic[OR_NUM][W];
int cp[20];
int cp1[7][18];
int v[18];
int ix[18];

int main()
{
	#ifdef DBG_OUT_FILE    
    FILE *fp_out;
	#endif
    int m1;
    int n1;
    int minr,maxr;
    int minc,maxc;
    int mind,maxd;
    int d;
    int r1,c1;
    int i,j,k,g,h,min,max;
    int Ccensus;
    int Csad;
    int disparityCost_c;
    int sum;
	unsigned char *ptr_uch=NULL;
	int *ptr_sint=NULL;
	
	#ifdef UART_EN
    WRT_GPIO(GPIO_0_BASE+0x3,0xff);
    WRT_GPIO(GPIO_0_BASE+0x4,0xff);
    WRT_GPIO(GPIO_0_BASE+0x5,0xff); 
    WRT_GPIO(GPIO_0_BASE+0x0,0x11);
    uart_init();
    uart_putc('s');   //debug used 
    uart_putc('\n');
    uart_putc(5);
    uart_putc('\n');
    uart_putc((2+'0'));
    uart_putc('\n');
	#endif

#ifdef DP_EN
    for(m1 = 4-1; m1 <= (OR_NUM +3)-1; m1++)
    {
        for(n1 = 4-1; n1 <= (W - 3)-1; n1++)
        {
            cvL[0][m1][n1] = (pixel_left[m1  ][n1+1] > pixel_left[m1  ][n1-1])?1:0;
            cvL[1][m1][n1] = (pixel_left[m1+1][n1  ] > pixel_left[m1-1][n1  ])?1:0;
            cvL[2][m1][n1] = (pixel_left[m1-1][n1-1] > pixel_left[m1+1][n1+1])?1:0;
            cvL[3][m1][n1] = (pixel_left[m1+1][n1-1] > pixel_left[m1-1][n1+1])?1:0;
            cvL[4][m1][n1] = (pixel_left[m1  ][n1-2] > pixel_left[m1  ][n1+2])?1:0;
            cvL[5][m1][n1] = (pixel_left[m1-2][n1  ] > pixel_left[m1+2][n1  ])?1:0;
            cvL[6][m1][n1] = (pixel_left[m1+2][n1+2] > pixel_left[m1-2][n1-2])?1:0;
            cvL[7][m1][n1] = (pixel_left[m1-2][n1+2] > pixel_left[m1+2][n1-2])?1:0;

            cvR[0][m1][n1] = (pixel_right[m1  ][n1+1] > pixel_right[m1  ][n1-1])?1:0;
            cvR[1][m1][n1] = (pixel_right[m1+1][n1  ] > pixel_right[m1-1][n1  ])?1:0;
            cvR[2][m1][n1] = (pixel_right[m1-1][n1-1] > pixel_right[m1+1][n1+1])?1:0;
            cvR[3][m1][n1] = (pixel_right[m1+1][n1-1] > pixel_right[m1-1][n1+1])?1:0;
            cvR[4][m1][n1] = (pixel_right[m1  ][n1-2] > pixel_right[m1  ][n1+2])?1:0;
            cvR[5][m1][n1] = (pixel_right[m1-2][n1  ] > pixel_right[m1+2][n1  ])?1:0;
            cvR[6][m1][n1] = (pixel_right[m1+2][n1+2] > pixel_right[m1-2][n1-2])?1:0;
            cvR[7][m1][n1] = (pixel_right[m1-2][n1+2] > pixel_right[m1+2][n1-2])?1:0;                
        }
    }
    
	ptr_uch = Ddynamic[0]; //
	for(i=0; i<30*640; ++i)
	{
		*ptr_uch++ = 0;
	}
	// for(i= 0;i<=29;i++)
	// {	
		// for(j=0;j<=639;j++)
        // {
            // Ddynamic[i][j] = 0;
        // }
    // } 
    
    for (m1 = 0;m1 <= OR_NUM-1; m1++)
    {
        minr = MAX(1, m1+1-halfBlockSize);
        maxr = MIN(H, m1+1+halfBlockSize);
        
		ptr_sint = disparityCost[0];
		for(i=0; i<20*640; ++i)
		{
			*ptr_sint++ = 100000;
		}
        // for(j=0;j<=19;j++)
        // {
            // for(i= 0;i<=639;i++)        
            // {
                // disparityCost[i][j] = 100000;
            // }
        // }
        
        for(n1 = 0; n1 <= W-1; n1++)
        {
            minc = MAX(1, n1+1-halfBlockSize2);
            maxc = MIN(W, n1+1+halfBlockSize2);
            mind = 0;
            maxd = MIN(disparityRange-1, minc-1);            
            
                    
            for(d = mind; d <= maxd; d++)
            {
                disparityCost_c = 0;
                for(r1 = minr-1; r1 <= maxr-1; r1++)
                {            
                    for(c1 = (minc-1-d); c1 <= (maxc-1-d); c1++)
                    {                    
                        Ccensus =    (cvL[0][r1][c1+d]^cvR[0][r1][c1])
                                    +(cvL[1][r1][c1+d]^cvR[1][r1][c1])
                                    +(cvL[2][r1][c1+d]^cvR[2][r1][c1])
                                    +(cvL[3][r1][c1+d]^cvR[3][r1][c1])
                                    +(cvL[4][r1][c1+d]^cvR[4][r1][c1])
                                    +(cvL[5][r1][c1+d]^cvR[5][r1][c1])
                                    +(cvL[6][r1][c1+d]^cvR[6][r1][c1])
                                    +(cvL[7][r1][c1+d]^cvR[7][r1][c1]);
                        /////////////////////////////////////////////////////
                    /*    printf("Ccensus = %d\n",Ccensus);
                        getchar();*/
                        /////////////////////////////////////////////////////
                        Csad    = abs(pixel_left[r1][c1+d] - pixel_right[r1][c1]); 
                        //Csad[i][j]    = abs(Csad[i][j]); 
                        
                        disparityCost_c = disparityCost_c + 2*Ccensus + Csad;                        
                        //j ++;
                    }
                    //sum = sum + disparityCost_c;
                    //i ++;
                }
                disparityCost[n1][d] = disparityCost_c;
            }
        }
		ptr_sint = optimalIndices[0];
		for(i=0; i<20*640; ++i)
		{
			*ptr_sint++ = 0;
		}                
        // for(j=0;j<=19;j++)
            // for(i= 0;i<=639;i++)
                // optimalIndices[i][j] = 0; 
                    
        for(j = 0; j<=19; j++)
        {
            cp[j] = disparityCost[0][j];
        }

        cp1[0][0]  = CFINF;
        cp1[0][1]  = CFINF;
        cp1[1][0]  = CFINF;
        cp1[5][17] = CFINF;
        cp1[6][16] = CFINF;
        cp1[6][17] = CFINF;
        
       for( j = 1; j <= W-1; j ++)
       {
            
            for(i = 0;i<=15;i++) cp1[0][i+2] = cp[i  ]+3*128;
            for(i = 0;i<=16;i++) cp1[1][i+1] = cp[i  ]+2*128;
            for(i = 0;i<=17;i++) cp1[2][i  ] = cp[i  ]+128;
            for(i = 0;i<=17;i++) cp1[3][i  ] = cp[i+1];
            for(i = 0;i<=17;i++) cp1[4][i  ] = cp[i+2]+128;
            for(i = 0;i<=16;i++) cp1[5][i  ] = cp[i+3]+2*128;
            for(i = 0;i<=15;i++) cp1[6][i  ] = cp[i+4]+3*128;
            
            cp[0] = CFINF;
            cp[19] = CFINF;
            for(i = 0; i <= 17; i++)
            {
                min = cp1[0][i];
                v[i] = min;
                ix[i] = 1;
                for(k = 0; k <= 6; k++)
                {  
                    if(cp1[k][i] < min)
                    {
                        min = cp1[k][i];
                        v[i] = min;
                        ix[i] = k +1;
                    }
                }
                cp[i+1] = disparityCost[j][i+1] + v[i];
                optimalIndices[j][i+1] = i+2 + (ix[i]-4);
            }

       }
       
       min = cp[0];
       Ddynamic[m1][W - 1] = 1;
       for(i = 0; i <= 19; i++)
       {
           if(cp[i] < min)
           {
              min = cp[i];
              Ddynamic[m1][W - 1] = i + 1; 
           }          
       }


       for(k = W - 1; k >= 1; k--)
       {
           min = MIN(20,Ddynamic[m1][k]);
           max = MAX(1,min);
           Ddynamic[m1][k-1] = optimalIndices[k][max-1];
       }
    }

/////////////////////////////////////////////////
	ptr_uch=&Ddynamic[0][0];
	for(i=0; i<30*640; ++i)
	{
		(*ptr_uch)--;
		ptr_uch++;
	}

	#ifdef UART_EN
    WRT_GPIO(GPIO_0_BASE+0x0,0x00);
    for(i=0; i<30; ++i)
    {
        for(j=0; j<640; ++j)    
        {
            if(Ddynamic[i][j]>9)
            {
                uart_putc((Ddynamic[i][j]/10+'0'));
                uart_putc((Ddynamic[i][j]%10+'0'));
            }
            else
                uart_putc((Ddynamic[i][j]+'0'));

            WRT_GPIO(GPIO_0_BASE+0x0,0x01);
        }    
        uart_putc('\n');
    }
	#endif

	#ifdef DBG_OUT_FILE    
    fp_out = fopen("Ddynamic.txt","w");
    for(i = 0; i < 30; i++)
    {
        for(j = 0; j < 640; j++)
        {
            fprintf(fp_out,"%d",Ddynamic[i][j]);
        }
        fprintf(fp_out,"\n");
    }
    fclose(fp_out); 
	#endif
    /*fp_out = fopen("optimalIndices.txt","w");
    for(i = 0; i <= 639; i++)
    {
        for(j = 0; j <= 19; j++)
        {
            fprintf(fp_out,"%d",optimalIndices[i][j]);
            //fprintf(fp_out,"%20d",CFINF);
        }
          fprintf(fp_out,"\n");
    }
    fclose(fp_out); */
	
#endif
//    WRT_GPIO(GPIO_0_BASE+0x0,0xAA);

	#ifdef STDATA_EN
	arry_dynamic.ch[0]=1;
	arry_dynamic.ch[1]=0;
	arry_dynamic.ch[2]=0;
	arry_dynamic.ch[3]=0;
	arry_dynamic.addrinit=(unsigned int)&Ddynamic[0][0];
	arry_dynamic.len=30*640;
	#endif

	#ifdef UART_EN
	uart_putc('d');
	uart_putc('\n');
	uart_putnum(12345678);
	uart_putc('\n');
	uart_putnum(arry_dynamic.addrinit);
	uart_putc('\n');
	uart_putc('r');
	uart_putc('\n');
	uart_putnum((unsigned int)&arry_dynamic);
	uart_putc('e');
	#endif
    
	return 0;
}

void int_error(void)
{
	#ifdef UART_EN
	uart_putc('\n');
	uart_putc('i');
	uart_putc('n');
	uart_putc('t');
	uart_putc('e');
	uart_putc('r');
	uart_putc('r');
	#endif
    while(1);
}

void int_main(void)
{
	#ifdef UART_EN
	uart_putc('\n');
	uart_putc('i');
	uart_putc('n');
	uart_putc('t');
	uart_putc('m');
	uart_putc('a');
	uart_putc('i');
	uart_putc('n');
	#endif
    while(1);
} 
