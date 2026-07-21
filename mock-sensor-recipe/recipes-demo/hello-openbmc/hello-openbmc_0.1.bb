SUMMARY = "Hello OpenBMC demo"
DESCRIPTION = "A minimal hello-world executable included in an OpenBMC firmware image."
LICENSE = "CLOSED"

SRC_URI = "file://hello-openbmc.sh"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${UNPACKDIR}/hello-openbmc.sh ${D}${bindir}/hello-openbmc
}

