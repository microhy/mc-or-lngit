ifndef CROSS_COMPILE
CROSS_COMPILE = or32-elf-
endif

export	CROSS_COMPILE

AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
VMEM = sram.vmem

HEADER = board.h spr-defs.h

OR32OBJ = hyor_ramboot.or32

.PHONY:all
all:$(OR32OBJ)

$(OR32OBJ): reset.o main.o ram.ld
	$(LD) reset.o main.o -T ram.ld -o $@

reset.o: reset.S
	$(CC) -c $^ -o $@
	
main.o: main.c
	$(CC) -c $< -o $@

.PHONY:vmem
vmem:$(OR32OBJ).bin $(OR32OBJ).txt

$(OR32OBJ).bin:$(OR32OBJ)
	$(OBJCOPY) -O binary $< $@
	$$ORPSOC_HOME/sw/utils/bin2vmem $@ > $(VMEM)

$(OR32OBJ).txt:$(OR32OBJ)
	$(OBJDUMP) -d $< > $@ 
	##python hyexpram.py $@ 
	less $@


.PHONY:clean
clean:
	rm -f *.or32 *.o *.bin *.or32.txt *.o.txt
	rm -f *.vmem	
