/*
* For a bin file transport to header file 
* Must attention to endian for OR1200 and Microblaze
*                                ---- hymicro 2017-2-25
* 
*
**
*/

#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

//#define   BIN_NAME            "led_dp_uart_sz.bin"   /*debug use*/
#define   OUTFILE_NAME      "or32_instruct.h"

#define   endian_big2lit(val)        ( ((val&0x000000ff)<<24)|\
                                      ((val&0x0000ff00)<<8) |\
                                      ((val&0x00ff0000)>>8) |\
                                      ((val&0xff000000)>>24) )
                                                  
#define   endian_lit2big(val)        ( ((val&0x000000ff)<<24)|\
                                      ((val&0x0000ff00)<<8) |\
                                      ((val&0x00ff0000)>>8) |\
                                      ((val&0xff000000)>>24) )


int main(int argc, char *argv[]) {
    
    FILE *fpin=NULL;
    FILE *fpout=NULL;
    unsigned int or32_instr;
    int instr_count;
    char *BIN_NAME;
    
    if(argc!=2)
    {
        puts("<err> argc != 2 ");
        puts("<err> Please enter the *.bin file name");
        puts("<hlp> Such as : ./ultils_bin2header bootsz.bin");
        return -1;
    }
    BIN_NAME = argv[1];
    fpin = fopen(BIN_NAME,"rb");
    if(fpin==NULL)
    {
        puts("open bin file err");
        return -1;
    }

    fpout=fopen(OUTFILE_NAME,"w");
    if(fpout==NULL)
    {
        puts("open out file err");
        return -1;
    }
    
    fprintf(fpout,"#ifndef __OR32_INSTRUCT_H__\n");
    fprintf(fpout,"#define __OR32_INSTRUCT_H__\n\n");    
    fprintf(fpout,"const unsigned int or32_instruct[]={\n");
    
    instr_count = 0;
    while(fread(&or32_instr,4,1,fpin) == 1)
    {
        instr_count++;
        or32_instr = endian_lit2big(or32_instr);
        fprintf(fpout,"0x%08x, ",or32_instr);
        if( 0==(instr_count%4) )
        {
            fprintf(fpout,"\n");        
        }
        if(instr_count==1)
        {
            printf("or32_instr = %08x\n",or32_instr);
        }
    }
    
    fprintf(fpout,"}; /*%d elements*/\n",instr_count);
    fprintf(fpout,"\n#endif /*----end of file----*/\n");
    
    fclose(fpin);
    fclose(fpout);
    
    puts("ok");
    return 0;
}
