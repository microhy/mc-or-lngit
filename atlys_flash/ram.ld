
MEMORY
{
	ram     : ORIGIN = 0x000000, LENGTH = 0x400000 
	unuse   : ORIGIN = 0x400000, LENGTH = 0x100000 
	stmem   : ORIGIN = 0x500000, LENGTH = 0x300000 
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
		_bstart = . ; 
		*(.bss)
		_bend =  . ; 
	} > ram

	.stack (NOLOAD):
	{
		*(.stack)
		_stack_top = .;
	} > ram
	
	.heap (NOLOAD):
	{
		_hstart = . ;
		*(.heap)
		_hend = . ;
	} > ram

	. = 0x500000;	
	.stdata_mem_type (NOLOAD): 
	{
		/*. = ALIGN(4);*/
		*(.stdata_mem_type)
	} > stmem

}
