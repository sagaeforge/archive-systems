BOOTLOADER_DIR = 00_BootLoader

all: BootLoader Disk.img

BootLoader:
	@echo
	@echo ================ Build Boot Loader ================
	@echo

	make -C $(BOOTLOADER_DIR)

	@echo
	@echo ================ Build Complete ================
	@echo

Disk.img: $(BOOTLOADER_DIR)/BootLoader.bin
	@echo
	@echo ================ Disk Image Build Start ================
	@echo

	cp $(BOOTLOADER_DIR)/BootLoader.bin Disk.img

	@echo
	@echo ================ All Build Complete ================
	@echo

clean:
	make -C $(BOOTLOADER_DIR) clean
	rm -f Disk.img