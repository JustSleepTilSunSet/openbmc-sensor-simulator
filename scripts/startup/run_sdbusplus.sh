cd sdbusplus
meson setup build --prefix=/usr -Dtests=disabled -Dexamples=disabled
ninja -C build
sudo ninja -C build install
sudo ldconfig