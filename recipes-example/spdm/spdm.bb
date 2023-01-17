LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit pkgconfig meson

SRC_URI = " \
            file://responder.c;subdir=${S} \
            file://requester.c;subdir=${S} \
            file://meson.build;subdir=${S} \
          "

DEPENDS = "i2c-tools"
RDEPENDS:${PN} = "i2c-tools"

