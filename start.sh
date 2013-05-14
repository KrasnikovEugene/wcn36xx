adb shell "insmod /data/compat.ko"
adb shell "insmod /data/cfg80211.ko"
adb shell "insmod /data/mac80211.ko"
adb shell "insmod /data/wcn36xx.ko debug_mask=0xffffffff"

# Enable dynamic debugging
adb shell "echo file dxe.c +p > /sys/kernel/debug/dynamic_debug/control"
adb shell "echo file txrx.c +p > /sys/kernel/debug/dynamic_debug/control"
adb shell "echo file smd.c +pflmt > /sys/kernel/debug/dynamic_debug/control"
adb shell "echo 1 > /sys/kernel/debug/tracing/events/mac80211/enable"
adb shell "echo 'module cfg80211 +p' > /sys/kernel/debug/dynamic_debug/control"
adb shell "echo 'module mac80211 +p' > /sys/kernel/debug/dynamic_debug/control"

# INTERFACE UP
adb shell "netcfg wlan0 up"
#wpa_supplicant /system/bin/logwrapper /system/bin/wpa_supplicant -iwlan0 -Dnl80211 -c/data/misc/wifi/wpa_supplicant.conf -e/data/misc/wifi/entropy.bin

#adb shell "echo file dxe.c +pflmt > /sys/kernel/debug/dynamic_debug/control"
#adb shell "echo file dxe.c +p > /sys/kernel/debug/dynamic_debug/control"
#adb shell "iw dev wlan0 scan freq 2412"
#adb shell "iw dev wlan0 scan"
#adb shell "iw dev wlan0 connect kosmoskatten 2437 00:18:e7:8a:7d:9c"
adb shell "iw dev wlan0 connect ASUS_24GHz4 2452 54:04:a6:8f:e9:ce"
adb shell "iw dev wlan0 disconnect"
sleep 15
adb shell "iw dev wlan0 connect ASUS_24GHz4 2452 54:04:a6:8f:e9:ce"
adb shell "dhcpcd wlan0"
