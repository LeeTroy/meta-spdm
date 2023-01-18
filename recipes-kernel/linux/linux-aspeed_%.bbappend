FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://0001-dts-enable-mctp-controller-on-i2c4-and-i2c13.patch file://0002-mtd-spi-nor-support-gigadevice-2g-flash.patch file://0003-mac-fix-udp-performance-issue.patch"
SRC_URI += "file://mctp.cfg"

