ifndef CROSS_COMPILE
CROSS_COMPILE = or32-elf-
endif

export	CROSS_COMPILE

AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
STRIP = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
VMEM = sram.vmem
LIBGCC := $(shell $(CC) $(CFLAGS) -print-libgcc-file-name)
LDFLAGS+= $(LIBGCC)

HEADER = board.h spr-defs.h

EXE = led_dp_uart
OR32ELF = $(EXE).elf
OR32BIN = $(EXE).bin
OR32BINSZ= $(EXE)_sz.bin

.PHONY:all
all:$(OR32BINSZ)

$(OR32ELF): reset.o main.o uart.o
	$(LD)  $^ $(LDFLAGS) -T ram.ld -o $@

reset.o: reset.S board.h
	$(CC) -c $< -o $@

main.o:main.c
	$(CC)  -c $< -o $@

uart.o:uart.c uart.h board.h
	$(CC)  -c $< -o $@

$(OR32BIN):$(OR32ELF)
	$(OBJCOPY) -O binary --strip-debug $< $@

$(OR32BINSZ):$(OR32BIN)
	bin2binsizeword $< $@
	./utils/utils_bin2header $@	
	cp $@ /mnt/share
	cp or32_instruct.h /mnt/share

.PHONY:clean
clean:
	rm -f *.bin *.elf *.o *.txt 
