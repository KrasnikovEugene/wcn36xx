ifneq ($(KERNELRELEASE),)
	wcn36xx-objs		+= \
				main.o \
				dxe.o \
				txrx.o \
				smd.o

	obj-m := wcn36xx.o
else
	KLIB	:= $(OUT)/obj/KERNEL_OBJ/
	PWD	:= $(shell pwd)

default:
	$(MAKE) ARCH=arm CROSS_COMPILE=$(ANDROID_BUILD_TOP)/prebuilts/gcc/linux-x86/arm/arm-eabi-4.6/bin/arm-eabi- -C $(KLIB) SUBDIRS=$(PWD) modules

clean:
	rm -rf  *.ko *.mod.* *.o .*.o.d .*.cmd .tmp_versions Module.symvers *.order
endif



