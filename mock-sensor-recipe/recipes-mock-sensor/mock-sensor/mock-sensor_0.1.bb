SUMMARY = "Mock Sensor D-Bus Service"
DESCRIPTION = "A simple mock sensor daemon exposing a D-Bus property through sdbusplus"
LICENSE = "CLOSED"

SRC_URI = " \
    file://mock_sensor.cpp \
    file://mock-sensor.service \
"

S = "${UNPACKDIR}"
W = "${UNPACKDIR}"
inherit pkgconfig systemd

DEPENDS += " \
    boost \
    sdbusplus \
    systemd \
"

SYSTEMD_SERVICE:${PN} = "mock-sensor.service"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"

do_compile() {
    ${CXX} ${CXXFLAGS} ${CPPFLAGS} -std=c++23 \
        ${S}/mock_sensor.cpp \
        -o ${B}/mock-sensor \
        $(pkg-config --cflags --libs sdbusplus libsystemd) \
        ${LDFLAGS}
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${B}/mock-sensor ${D}${bindir}/mock-sensor

    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${S}/mock-sensor.service ${D}${systemd_system_unitdir}/mock-sensor.service
}
