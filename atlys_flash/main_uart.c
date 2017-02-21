#include <stdio.h>
//#include <stdint.h>
//#include <math.h>
//#include <limits.h>


#include"board.h"
#include"uart.h"
 
#define  WRT_GPIO(addr,val)  (*(unsigned char *)(addr)=(val))
#define   DBG_OUT_FILE   0 
#define   DP_EN
#ifdef   DP_EN
	#include "pixel_left.h"
	#include "pixel_right.h"
#endif

#define   H        (480)
#define   W        (640)
#define   OR_NUM   (30)		//60->30 
#define   disparityRange     (20)
#define   halfBlockSize      (3)
#define   halfBlockSize2     (3)
#define   MAX(a,b) ((a)>(b))?(a):(b)
#define   MIN(a,b) ((a)<(b))?(a):(b)

	unsigned short int cvL[8][33][640] = {{0,0,0}};
	unsigned short int cvR[8][33][640] = {{0,0,0}};

	int disparityCost[W][20];
	int optimalIndices[W][20];

	unsigned char Ddynamic[OR_NUM][W];

int main()
{
/////////////////////////////////////////////////////////////////////////////////////
// 第一个or计算任务，像素缓冲（1-35）行，计算出1-33行的sensus变换
// 动态规划 m（1-30）变化，r（1-33）
/////////////////////////////////////////////////////////////////////////////////////	
#if DBG_OUT_FILE	
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
	//int Penalty = 128;
	int Ccensus;
	int Csad;
	int disparityCost_c;
	int sum;
	int cp[20];
	int cp1[7][18];
	int cfinf;
	int v[18];
	int ix[18];
//	int *ptr_wr=(int *)0x0000_1000;

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
#ifdef DP_EN
	for(m1 = 4-1; m1 <= (OR_NUM +3)-1; m1++) //像素缓冲35行（1-35行）,计算出1-33行的sencus变换
	//for(m1 = 4-1; m1 <= 62; m1++) //像素缓冲35行（1-35行）,计算出1-33行的sencus变换
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
	////////////////////////////////////
	//fp_out = fopen("CVR.txt","w");
	//for(i = 0; i <= 7; i++)
	//	for(j = 0; j <= 32; j++)
	//		for(k = 0; k <= 639; k++)
	//		{
	//			fprintf(fp_out,"%d\n",cvR[i][j][k]);
	//		}
	//	   //getchar();
	//fclose(fp_out);
	////////////////////////////////////
		
	for(j=0;j<=639;j++)
	{
		for(i= 0;i<=29;i++)
		{
			Ddynamic[i][j] = 0;
		}
	} 
	
	for (m1 = 0;m1 <= OR_NUM-1; m1++)
//	for (m1 = 0;m1 <= 59; m1++)
	{
		minr = MAX(1, m1+1-halfBlockSize);
		maxr = MIN(H, m1+1+halfBlockSize);
		
		for(j=0;j<=19;j++)
		{
			for(i= 0;i<=639;i++)		
			{
				disparityCost[i][j] = 100000;
			}
		}
		
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
					/*	printf("Ccensus = %d\n",Ccensus);
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
				
		for(j=0;j<=19;j++)
			for(i= 0;i<=639;i++)
				optimalIndices[i][j] = 0; 
					
		for(j = 0; j<=19; j++)
		{
			cp[j] = disparityCost[0][j];
		}
		cfinf = 1000000000;
		//cfinf = 1000000;
		//cfinf = INT_MAX;

		cp1[0][0]  = cfinf;
		cp1[0][1]  = cfinf;
		cp1[1][0]  = cfinf;
		cp1[5][17] = cfinf;
		cp1[6][16] = cfinf;
		cp1[6][17] = cfinf;
		
	   for( j = 1; j <= W-1; j ++)
	   {
		/* int cp1[7][18] = {
			        {cfinf       ,cfinf       ,cp[0 ]+3*128,cp[1 ]+3*128,cp[2 ]+3*128,cp[3 ]+3*128,cp[4 ]+3*128,cp[5 ]+3*128,cp[6 ]+3*128,cp[7 ]+3*128,cp[8 ]+3*128,cp[9 ]+3*128,cp[10]+3*128,cp[11]+3*128,cp[12]+3*128,cp[13]+3*128,cp[14]+3*128,cp[15]+3*128},
					{cfinf       ,cp[0 ]+2*128,cp[1 ]+2*128,cp[2 ]+2*128,cp[3 ]+2*128,cp[4 ]+2*128,cp[5 ]+2*128,cp[6 ]+2*128,cp[7 ]+2*128,cp[8 ]+2*128,cp[9 ]+2*128,cp[10]+2*128,cp[11]+2*128,cp[12]+2*128,cp[13]+2*128,cp[14]+2*128,cp[15]+2*128,cp[16]+2*128},
					{cp[0 ]+128  ,cp[1 ]+128  ,cp[2 ]+128  ,cp[3 ]+128  ,cp[4 ]+128  ,cp[5 ]+128  ,cp[6 ]+128  ,cp[7 ]+128  ,cp[8 ]+128  ,cp[9 ]+128  ,cp[10]+128  ,cp[11]+128  ,cp[12]+128  ,cp[13]+128  ,cp[14]+128  ,cp[15]+128  ,cp[16]+128  ,cp[17]+128  },
					{cp[1 ]      ,cp[2 ]      ,cp[3 ]      ,cp[4 ]      ,cp[5 ]      ,cp[6 ]      ,cp[7 ]      ,cp[8 ]      ,cp[9 ]      ,cp[10]      ,cp[11]      ,cp[12]      ,cp[13]      ,cp[14]      ,cp[15]      ,cp[16]      ,cp[17]      ,cp[18]      },
					{cp[2 ]+128  ,cp[3 ]+128  ,cp[4 ]+128  ,cp[5 ]+128  ,cp[6 ]+128  ,cp[7 ]+128  ,cp[8 ]+128  ,cp[9 ]+128  ,cp[10]+128  ,cp[11]+128  ,cp[12]+128  ,cp[13]+128  ,cp[14]+128  ,cp[15]+128  ,cp[16]+128  ,cp[17]+128  ,cp[18]+128  ,cp[19]+128  },
					{cp[3 ]+2*128,cp[4 ]+2*128,cp[5 ]+2*128,cp[6 ]+2*128,cp[7 ]+2*128,cp[8 ]+2*128,cp[9 ]+2*128,cp[10]+2*128,cp[11]+2*128,cp[12]+2*128,cp[13]+2*128,cp[14]+2*128,cp[15]+2*128,cp[16]+2*128,cp[17]+2*128,cp[18]+2*128,cp[19]+2*128,cfinf       },
					{cp[4 ]+3*128,cp[5 ]+3*128,cp[6 ]+3*128,cp[7 ]+3*128,cp[8 ]+3*128,cp[9 ]+3*128,cp[10]+3*128,cp[11]+3*128,cp[12]+3*128,cp[13]+3*128,cp[14]+3*128,cp[15]+3*128,cp[16]+3*128,cp[17]+3*128,cp[18]+3*128,cp[19]+3*128,cfinf       ,cfinf       }
				};  */
			
			for(i = 0;i<=15;i++) cp1[0][i+2] = cp[i  ]+3*128;
			for(i = 0;i<=16;i++) cp1[1][i+1] = cp[i  ]+2*128;
			for(i = 0;i<=17;i++) cp1[2][i  ] = cp[i  ]+128;
			for(i = 0;i<=17;i++) cp1[3][i  ] = cp[i+1];
			for(i = 0;i<=17;i++) cp1[4][i  ] = cp[i+2]+128;
			for(i = 0;i<=16;i++) cp1[5][i  ] = cp[i+3]+2*128;
			for(i = 0;i<=15;i++) cp1[6][i  ] = cp[i+4]+3*128;
			
			cp[0] = cfinf;
			cp[19] = cfinf;
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

			/* for(g = 0; g <= 19; g++)
			{
				if(g==0 || g==19)
					cp[g] = cfinf;
				else 
					cp[g] = disparityCost[j][g] + v[g-1];
			} */
	
							 
			/* for(h = 1; h<=18; h++)
			{
				optimalIndices[j][h] = h+1 + (ix[h-1]-4);
			} */
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
		  /*  else
		   {
			  min = min;
			  Ddynamic[m1][W - 1] = Ddynamic[m1][W - 1]; 
		   } */ 		   
	   }


	   for(k = W - 1; k >= 1; k--)
	   {
		   min = MIN(20,Ddynamic[m1][k]);
		   max = MAX(1,min);
		   Ddynamic[m1][k-1] = optimalIndices[k][max-1];
		   
		   //getchar();
	   }
	}

/////////////////////////////////////////////////
/*
#if DBG_OUT_FILE	
 	fp_out = fopen("Ddynamic.txt","w");
#endif
	for(i = 0; i <= 59; i++)
	{
		for(j = 0; j <= 639; j++)
		{
			Ddynamic[i][j] = Ddynamic[i][j] - 1;
			#if DBG_OUT_FILE	
			fprintf(fp_out,"%d",Ddynamic[i][j]);
			#endif
		}
		#if DBG_OUT_FILE	
		fprintf(fp_out,"\n");
		#endif
	}
#if DBG_OUT_FILE	
	fclose(fp_out); 
#endif
*/
    WRT_GPIO(GPIO_0_BASE+0x0,0x00);
	for(i=0; i<30; ++i)
	{
		for(j=0; j<640; ++j)	
		{
			Ddynamic[i][j]--;
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
	/*fp_out = fopen("optimalIndices.txt","w");
	for(i = 0; i <= 639; i++)
	{
		for(j = 0; j <= 19; j++)
		{
			fprintf(fp_out,"%d",optimalIndices[i][j]);
			//fprintf(fp_out,"%20d",cfinf);
		}
		  fprintf(fp_out,"\n");
	}
	fclose(fp_out); */
/////////////////////////////////////////////////
	 
/////////////////////////////////////////////////////////////////////////////////////
// 第二个or计算任务，像素缓冲（26-65）行，计算出第28-63行的sensus变换
// 动态规划 m（31-60）变化
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
// 第三个or计算任务，像素缓冲（26-65）行，计算出第28-63行的sensus变换
// 动态规划 m（31-60）变化
/////////////////////////////////////////////////////////////////////////////////////

	//for (m2 = OR_NUM + 1;m2 <= OR_NUM + 30; m2++)//the second OR(second 30 line)缓冲第二个30行
	//{

	//}
#endif
    WRT_GPIO(GPIO_0_BASE+0x0,0xAA);
	return 0;
} 
