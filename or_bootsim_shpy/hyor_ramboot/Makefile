ifndef CROSS_COMPILE
CROSS_COMPILE = or32-elf-
endif

export	CROSS_COMPILE

AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJDUMP = $(CROSS_COMPILE)objdump

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
vmem:$(OR32OBJ).txt

$(OR32OBJ).txt:$(OR32OBJ)
	$(OBJDUMP) -d $< > $@ 
	python hyexpram.py $@ 

.PHONY:clean
clean:
	rm -f *.or32 *.o
	rm -f *.or32.txt *.o.txt
	rm -f *.vmem
	
