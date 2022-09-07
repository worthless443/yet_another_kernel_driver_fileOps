VERSION=5.18.16-arch1-1
ifndef DRIVER
DRIVER=driver2.o
endif
obj-m:=$(DRIVER)
KERN_PATH = /lib/modules/$(shell uname -r)/build M=$(shell pwd) 

BUILD:
	make -C $(KERN_PATH) modules
