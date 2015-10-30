all: bootloader kernel32 disk.img

bootloader:
	@echo
	@echo ========== Build Boot Loader ==========
	@echo

	make -C bootloader

	@echo
	@echo ========== Build Complete ==========
	@echo

kernel32:
	@echo
	@echo ========== Build 32bit Kernel ==========
	@echo

	make -C kernel32

	@echo
	@echo
	@echo ========== Build Complete ==========

disk.img: bootloader/bootloader.bin kernel32/kernel32.bin
	@echo 
	@echo ========== Disk Image Build Start ==========
	@echo

	./utility/imagemaker $^

	@echo
	@echo ========== All Build Complete ==========
	@echo

clean:
	make -C bootloader clean
	rm -f disk.img
