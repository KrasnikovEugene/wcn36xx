
Build Instructions for CyanogenMod+Mint

1) Follow instructions (http://wiki.cyanogenmod.org/w/Build_for_mint)
   do download and build CM sources.

2) In case of build error "No such file or directory:
   'vendor/sony/blue-common/proprietary/boot/RPM.bin'" download file
   RPM.bin from https://github.com/TheMuppets/proprietary_vendor_sony
   and put it to the folder 'vendor/sony/blue-common/proprietary/boot'

3) Flash built image to the phone as described here
   http://www.xperiablog.net/2012/12/04/how-to-install-cyanogenmod-10-on-your-sony-xperia-t-guide/

4) Download wcn36xx sources from https://github.com/KrasnikovEugene/wcn36xx

5) cd wcn36xx

6) make

Build Instructions for CyanogenMod+Mako

1) Follow instructions (http://wiki.cyanogenmod.org/w/Build_for_mako)
   do download and build CM sources.

3) Flash the built image to the phone

4) Download wcn36xx sources from https://github.com/KrasnikovEugene/wcn36xx

5) cd wcn36xx

6) Run make (must happen in the same session where breakfast is run)

Hardcoding makefile paths

If you don't want to use Android build scripts to set the build
environment variables you can hardcode them in the .config file:

ANDROID_BUILD_TOP=/home/foo/mako/cm-10.1
CM_BUILD=mako

ARM_EABI_TOOLCHAIN=$(ANDROID_BUILD_TOP)/prebuilts/gcc/linux-x86/arm/arm-eabi-4.6/bin
ANDROID_PRODUCT_OUT=$(ANDROID_BUILD_TOP)/out/target/product/$(CM_BUILD)

Mailing list: wcn36xx@lists.infradead.org
Subscription: http://lists.infradead.org/mailman/listinfo/wcn36xx
