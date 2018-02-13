KERNROOT := /lib/modules/`uname -r`/build

nvme:
	$(MAKE) -C $(KERNROOT) M=`pwd`/drivers/block

clean:
	$(MAKE) -C $(KERNROOT) M=`pwd`/drivers/block clean

headers_install:
	@ cp include/uapi/linux/* /usr/include/linux/

.PHONY: all clean nvme headers_install
