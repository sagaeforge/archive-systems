
BOOTLOADER_DIR = 00_BootLoader
KERNEL32_DIR = 01_Kernel32

all: BootLoader Kernel32 Disk.img

BootLoader:
	@echo
	@echo ================ Build Boot Loader ================
	@echo

	make -C $(BOOTLOADER_DIR)

	@echo
	@echo ================ Build Complete ================
	@echo

Kernel32:
	@echo
	@echo ================ Build 32bit kernel ================
	@echo

	make -C $(KERNEL32_DIR)

	@echo
	@echo ================ Build Complete ================
	@echo

Disk.img: $(BOOTLOADER_DIR)/BootLoader.bin $(KERNEL32_DIR)/Kernel32.bin
	@echo
	@echo ================ Disk Image Build Start ================
	@echo

	cat $^ > Disk.img

	@echo
	@echo ================ All Build Complete ================
	@echo

clean:
	make -C $(BOOTLOADER_DIR) clean
	make -C $(KERNEL32_DIR) clean
	rm -f Disk.img