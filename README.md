Build Instructions for CyanogenMod+Mint
1) Follow instructions (http://wiki.cyanogenmod.org/w/Build_for_mint) do download and build CM sources.
2) In case of build error "No such file or directory: 'vendor/sony/blue-common/proprietary/boot/RPM.bin'" download file RPM.bin from https://github.com/TheMuppets/proprietary_vendor_sony and put it to the folder 'vendor/sony/blue-common/proprietary/boot'
3) Flash built image to the phone as described here http://www.xperiablog.net/2012/12/04/how-to-install-cyanogenmod-10-on-your-sony-xperia-t-guide/
4) Download wcn36xx sources from https://github.com/KrasnikovEugene/wcn36xx
5) In Makefile(https://github.com/KrasnikovEugene/wcn36xx/blob/master/Makefile) set ROOT_PATH to the folder where CM sources are located from bullet 1.
6) Type 'make' that will produce “wcn36xx.ko”
