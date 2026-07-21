## Troubleshooting / FAQ

### Q1: How to check the Arduino serial output?

Check [verification.md #7](verification.md).

---

### Q2: Why does `arduino-cli monitor` show that the serial port is busy after starting OpenBMC QEMU?

Because OpenBMC QEMU may already be opening the host serial device, such as `/dev/ttyACM0` or `/dev/ttyACM1`.

A serial device should not be monitored by multiple readers at the same time.
If QEMU is already using the device, running `arduino-cli monitor` from the host may show that the serial port is busy.

Use the following command to check which process owns the serial device:

```bash
sudo lsof /dev/ttyACM0
```

or:

```bash
sudo fuser -v /dev/ttyACM0
```

If the owner is a QEMU process, stop QEMU before using `arduino-cli monitor`.

---

### Q3: How should the LM75 I2C pull-up resistors be connected?

Use 4.7kΩ pull-up resistors on both SDA and SCL lines.

Recommended wiring:

```text
Arduino 3.3V  ───────── LM75 VCC
Arduino GND   ───────── LM75 GND

Arduino SDA   ───────── LM75 SDA
Arduino SCL   ───────── LM75 SCL

3.3V ── 4.7kΩ ── SDA line
3.3V ── 4.7kΩ ── SCL line
```

Notes:

- SDA and SCL are I2C bus lines.
- The resistors are pull-up resistors, not series resistors.
- Use the same voltage level for LM75 VCC and the I2C pull-up voltage.
- 3.3V is recommended unless the entire I2C bus is confirmed to be 5V-safe.
