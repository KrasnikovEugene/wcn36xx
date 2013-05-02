adb shell "insmod /data/compat.ko"
adb shell "insmod /data/cfg80211.ko"
adb shell "insmod /data/mac80211.ko"
adb shell "insmod /data/wcn36xx.ko"

adb shell "echo file dxe.c +p > /sys/kernel/debug/dynamic_debug/control"
adb shell "echo file txrx.c +p > /sys/kernel/debug/dynamic_debug/control"
adb shell "echo file smd.c +pflmt > /sys/kernel/debug/dynamic_debug/control"
adb shell "echo 1 > /sys/kernel/debug/tracing/events/mac80211/enable"
adb shell "echo 'module cfg80211 +p' > /sys/kernel/debug/dynamic_debug/control"
adb shell "echo 'module mac80211 +p' > /sys/kernel/debug/dynamic_debug/control"

adb shell "hostapd /data/hostapd.conf"

#Where hostapd.conf contains
#interface=wlan0
#driver=nl80211
#ssid=test
#channel=1
