MEMORY
        {
/*        vectors : ORIGIN = 0x00000000, LENGTH = 0x0000100 */
	/* Use all RAM */
	/* Put all sections into ram */
        ram     : ORIGIN = 0x0000000, LENGTH = 0x1800 
        stmem     : ORIGIN = 0x1c00, LENGTH = 0x400 
       /* vectors : ORIGIN = 0x00000000, LENGTH = 0x00002000 */
	/* Use all RAM */
	/* Put all sections into ram */
	
       /* ram     : ORIGIN = 0x00002000, LENGTH = 0x02000000 - 0x00002000*/
	
	/* Uncomment the following to work around bad pages in a flash mem */
	/* Put all sections except data, rodata into ram2 */
	/* On ORSoC dev board devices, for pages 132-134 (256Byte pages) */
	/*
        ram     : ORIGIN = 0x00001200, LENGTH = 0x8400 - 0x1200
        ram2     : ORIGIN = 0x8700, LENGTH = 0x02000000 - 0x8700
	*/
	/* Uncomment the following to work around bad pages in a flash mem */
	/* Put all sections except data, rodata into ram2 */
	/* On ORSoC dev board devices, for pages 180-182 (256Byte pages) */
	/* 
        ram     : ORIGIN = 0x00001200, LENGTH = 0xB400 - 0x1200
        ram2     : ORIGIN = 0xb700, LENGTH = 0x02000000 - 0xb700
	*/
	/*flash   : ORIGIN = 0xf0000000, LENGTH = 0x04000000*/
        }



SECTIONS
{
        .vectors :
        {
        *(.vectors)
        } > ram

        .text :
        {
	_text_begin = .;
        *(.text)
	_text_end = .;
	} > ram
/*        } > ram2*/

        .data :
        {
        *(.data)
        } > ram

        .rodata :
        {
        *(.rodata)
	*(.rodata.*)
        } > ram

        .bss (NOLOAD):
        {
	. = ALIGN(4); 
	_bstart = . ; 
        *(.bss)
	_bend =  . ; 
	} > ram
/*        } > ram2 */

        .stack (NOLOAD):
        {
        *(.stack)
        _stack_top = .;
	} > ram
/*        } > ram2 */

		. = 0x1c00;
	 	.stdata_mem_type : 
		{
			*(.stdata_mem_type)
		} > stmem

}
