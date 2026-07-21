#!bin/sh
# Turn on QEMU virtual machine.
cd ~/openbmc
qemu-system-arm \
  -M romulus-bmc \
  -m 512 \
  -nographic \
  -drive file=./build/romulus/tmp/deploy/images/romulus/obmc-phosphor-image-romulus.static.mtd,format=raw,if=mtd \
  -net nic \
  -net user,hostfwd=tcp::2222-:22,hostfwd=tcp::2443-:443 \
  -serial mon:stdio \
  -chardev serial,id=arduino0,path=/dev/ttyACM1 \
  -serial chardev:arduino0