# Verification

This document lists the verified functions and the commands used to validate them.

## OpenBMC QEMU

### 1. Start OpenBMC QEMU

- [x] The startup script is provided:

```text
scripts/startup/startup_bmc.sh
```

Run on the development VM:

```bash
./scripts/startup/startup_bmc.sh
```

Expected result:

- OpenBMC QEMU starts successfully.
- The user can enter the QEMU console.

To leave QEMU when using `-nographic`:

```text
Ctrl + A, then X
```

---

### 2. Forward OpenBMC simulator port

- [x] The OpenBMC simulator port can be forwarded from the VM to the host machine by using SSH local port forwarding.

Run on the host machine:

```bash
ssh -N -L <your-local-port>:127.0.0.1:<bmc-port> <your-username>@<your-vm-ip>
```

Notes:

- `127.0.0.1` refers to the loopback interface inside the virtual machine.
- `<your-local-port>` is the port exposed on the host machine.
- `<bmc-port>` is the OpenBMC simulator port inside the VM.

Expected result:

- The OpenBMC Web / Redfish service can be accessed from the host machine through `https://127.0.0.1:<your-local-port>`.

---

### 3. Basic Redfish check

- [x] The basic Redfish endpoint is reachable.

Run on the host machine:

```bash
curl -k -u <user-name>:<user-password> \
  https://127.0.0.1:<port>/redfish/v1
```

Expected result:

- HTTP status code is `200`.
- The response contains the standard Redfish / OData root object.

Optional HTTP status check:

```bash
curl -k -u <user-name>:<user-password> https://127.0.0.1:<port>/redfish/v1
```

Expected result:

- HTTP status code is 200.

---

### 4. Verify Arduino LM75 sensor from Redfish

- [x] The Arduino LM75 temperature sensor value is exposed through the OpenBMC Redfish endpoint.

Run on the host machine:

```bash
curl -k -u <user-name>:<user-password> \
  https://127.0.0.1:<port>/redfish/v1/Chassis/chassis/Sensors/temperature_arduino_lm75
```

Expected result:

- HTTP status code is `200`.
- The response contains the Arduino LM75 temperature value.

---

### 5. Verify D-Bus sensor value

- [x] The Arduino LM75 sensor value is registered on D-Bus.

Run inside the OpenBMC QEMU guest:

```bash
busctl get-property \
  xyz.openbmc_project.MockSensor \
  /xyz/openbmc_project/sensors/temperature/arduino_lm75 \
  xyz.openbmc_project.Sensor.Value \
  Value
```

Expected result:

- The command returns a valid temperature value.
- Example:

```text
d 26.5
```

---

### 6. Verify D-Bus object tree

- [x] The mock sensor service exposes the expected object path.

Run inside the OpenBMC QEMU guest:

```bash
busctl tree xyz.openbmc_project.MockSensor
```

Expected result:

```text
`-/xyz
  `-/xyz/openbmc_project
    `-/xyz/openbmc_project/sensors
      `-/xyz/openbmc_project/sensors/temperature
        `-/xyz/openbmc_project/sensors/temperature/arduino_lm75
```

---

### 7. Discover serial device inside OpenBMC

- [x] The Arduino serial output can be discovered from the OpenBMC QEMU guest.

Run inside the OpenBMC QEMU guest:

```bash
dmesg | grep -Ei "ttyS|uart|serial|vuart"
```

Expected result:

- The kernel log shows available serial devices, such as `ttyS0`, `ttyS4`, or `ttyS5`.
- The Arduino passthrough device is expected to appear as one of the `ttyS*` devices inside QEMU.

Check serial output:

```bash
for d in /dev/ttyS*; do
  echo "===== $d ====="
  stty -F "$d" 115200 raw -echo -crtscts 2>/dev/null || continue
  timeout 5 cat "$d"
done
```

Expected result:

- One serial device prints the LM75 temperature value.
- Example:

```text
TEMP_C=26.5
```

Note:

- Some OpenBMC images may not provide the `timeout` command.
- If `timeout` is unavailable, test the device manually:

```bash
stty -F /dev/ttyS0 115200 cs8 -cstopb -parenb -ixon -ixoff raw -echo
cat /dev/ttyS0
```

Expected result:

```text
TEMP_C=<temperature-value>
```

## Development Environment

### 1. File synchronization

- [x] File synchronization is implemented with the `unison` package.

Expected result:

- Source files can be synchronized between the host machine and the development VM.

---

### 2. SSH access to VM

- [x] The development VM can be accessed through SSH.

Run on the host machine:

```bash
ssh <your-username>@<your-vm-ip>
```

Expected result:

- The user can log in to the development VM.

---

### 3. Docker shared folder

- [x] The Docker container can access the shared project folder.

Expected result:

- The source code can be built or modified inside the Docker container without polluting the host machine environment.

## Arduino

### 1. Verify arduino cli completely install

- [x] Check the develop tool installiation finished.

```bash
arduino-cli board list
```

Expected result:

- The Arduino board is listed.
- The serial device is shown, for example /dev/ttyACM0.

expect: Display your board and serial device.

### 2. Verify Arduino sketch compilation and upload

- [x] Check that the Arduino sketch can be compiled and uploaded successfully.

```bash
arduino-cli compile   --fqbn <your-board-fqbn>   .
arduino-cli upload   -p /dev/ttyACM0   --fqbn <your-board-fqbn>   .
```

Expected result:

- Compilation finishes without errors.
- Upload finishes successfully.
- The board runs the uploaded sketch.

### 3. Verify LM75 sensor value from Arduino serial monitor

- [x] The Arduino can read the LM75 sensor value and print it through serial output.

Run on the host machine:

```bash
arduino-cli monitor -p /dev/ttyACM0 --config baudrate=115200
```

Expected result:

```text
TEMP_C=<temperature-value>
```

Example:

```text
TEMP_C=26.5
```

Note:

- If QEMU is already using the Arduino serial device, this command may report that the serial port is busy.
- In that case, stop QEMU first or verify the value from the corresponding `/dev/ttyS*` device inside the OpenBMC QEMU guest.
