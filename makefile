
BOOTLOADER_DIR = 00_BootLoader
KERNEL32_DIR = 01_Kernel32
KERNEL64_DIR = 02_Kernel64
UTILITY_DIR = 04_Utility

all: BootLoader Kernel32 Kernel64 Disk.img Utility

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

Kernel64:
	@echo
	@echo ================ Build 64bit kernel ================
	@echo

	make -C $(KERNEL64_DIR)

	@echo
	@echo ================ Build Complete ================
	@echo

Disk.img: $(BOOTLOADER_DIR)/BootLoader.bin $(KERNEL32_DIR)/Kernel32.bin $(KERNEL64_DIR)/Kernel64.bin
	@echo
	@echo ================ Disk Image Build Start ================
	@echo

	./ImageMaker $^

	@echo
	@echo ================ All Build Complete ================
	@echo

# 유틸리티 빌드
Utility:
	@echo
	@echo =========== Utility Build Start ===========
	@echo

	make -C $(UTILITY_DIR)

	@echo
	@echo =========== Utility Build Complete ===========
	@echo
clean:
	make -C $(BOOTLOADER_DIR) clean
	make -C $(KERNEL32_DIR) clean
	make -C $(KERNEL64_DIR) clean
	rm -f Disk.img