DIR := pidinfo
VERSION := 4.16.0
SRC_PATH := /usr/src/linux-$(VERSION)

all:
	rm -rf $(SRC_PATH)/$(DIR)
	cp -r $(DIR) $(SRC_PATH)/$(DIR)
	make -C $(SRC_PATH)/ -j04
	cp -v $(SRC_PATH)/arch/x86/boot/bzImage /boot/vmlinuz-$(VERSION)-$(DIR)
	cp -v $(SRC_PATH)/System.map /boot/System.map-$(VERSION)-$(DIR)
	cp -v $(SRC_PATH)/.config /boot/config-$(VERSION)-$(DIR)
	reboot
