# make it possible to set/override android variables in .config
-include .config

CROSS_COMPILE?=$(ARM_EABI_TOOLCHAIN)/arm-eabi-

ifneq ($(KERNELRELEASE),)
	wcn36xx_msm-objs		+= \
				main.o

	obj-m := wcn36xx_msm.o
else
	KLIB_BUILD := $(ANDROID_PRODUCT_OUT)/obj/KERNEL_OBJ/
	PWD	:= $(shell pwd)

default:
	$(MAKE) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) -C $(KLIB_BUILD) SUBDIRS=$(PWD) modules

clean:
	rm -rf  *.ko *.mod.* *.o .*.o.d .*.cmd .tmp_versions Module.symvers *.order \
		cscope.out

cscope:
	cscope -b

endif



