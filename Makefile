ROOT_PATH := <root>
ifneq ($(KERNELRELEASE),)
	wcn36xx-objs		+= \
				main.o \
				dxe.o \
				txrx.o \
				smd.o

	obj-m := wcn36xx.o
else
	KLIB    := $(ROOT_PATH)/out/target/product/mint/obj/KERNEL_OBJ/
	PWD	:= $(shell pwd)

default:
	$(MAKE) ARCH=arm CROSS_COMPILE=$(ROOT_PATH)/prebuilts/gcc/linux-x86/arm/arm-eabi-4.6/bin/arm-eabi- -C $(KLIB) SUBDIRS=$(PWD) modules

clean:
	rm -rf  *.ko *.mod.* *.o .*.o.d .*.cmd .tmp_versions Module.symvers *.order \
		cscope.out

cscope:
	cscope -b

endif



