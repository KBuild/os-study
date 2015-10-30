NASM32 = nasm
GCC32 = gcc -c -m32 -ffreestanding
LD32 = ld -melf_i386 -T ../elf_i386.x -nostdlib -e main -Ttext 0x10200
OBJCOPY32 = objcopy -j .text -j .data -j .rodata -j .bss -S -O binary

OBJDIR = temp
SRCDIR = source

all: prepare kernel32.bin

prepare:
	mkdir -p $(OBJDIR)

$(OBJDIR)/entrypoint.bin: $(SRCDIR)/entrypoint.s
	$(NASM32) -o $@ $<

dep:
	@echo === Make Dependency File ===
	make -C $(OBJDIR) -f ../Makefile internaldep
	@echo === Dependency Search Complete

executeinternalbuild: dep
	make -C $(OBJDIR) -f ../Makefile kernel32.elf

$(OBJDIR)/kernel32.elf.bin: executeinternalbuild
	$(OBJCOPY32) $(OBJDIR)/kernel32.elf $@

kernel32.bin: $(OBJDIR)/entrypoint.bin $(OBJDIR)/kernel32.elf.bin
	cat $^ > $@

clean:
	rm -f *.bin
	rm -f $(OBJDIR)/*.*

CENTRYPOINTOBJFILE = main.o
CSRCFILES = $(wildcard ../$(SRCDIR)/*.c)
ASMSRCFILES = $(wildcard ../$(SRCDIR)/*.asm)
COBJFILES = $(subst main.o, , $(notdir $(patsubst %.c,%.o,$(CSRCFILES))))
ASMOBJFILES = $(notdir $(patsubst %.asm,%.o,$(ASMSRCFILES)))

%.o: ../$(SRCDIR)/%.c
	$(GCC32) -c $<

%.o: ../$(SRCDIR)/%.asm
	$(NASM32) -f elf32 -o $@ $<

internaldep:
	$(GCC32) -MM $(CSRCFILES) > dependency.dep

kernel32.elf: $(CENTRYPOINTOBJFILE) $(COBJFILES) $(ASMOBJFILES)
	$(LD32) -o $@ $^

ifeq (dependency.dep, $(wildcard dependency.dep))
include dependency.dep
endif
