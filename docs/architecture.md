# Architecture

## Definition

| Layer                      | Role                                            |
| -------------------------- | ----------------------------------------------- |
| Host machine               | Local development machine                       |
| Development VM / Container | Build OpenBMC or dependencies                   |
| OpenBMC QEMU               | Runtime environment for testing D-Bus / Redfish |

## Develop environment.

```
Host
  └── Development Environment
        └── OpenBMC QEMU
              └── Mock Sensor Service
                    └── D-Bus / Redfish
```

## Data flow:

```text
[LM75]
    |
    | I2C interface
    |
    v
[Arduino]
    |
    | Serial output
    | Example: TEMP_C=26.0
    v
[Host /dev/ttyACM0]
    |
    | QEMU serial passthrough
    v
[OpenBMC QEMU /dev/ttyS0]
    |
    | Read by mock sensor daemon
    v
[xyz.openbmc_project.MockSensor]
    |
    | Register sensor value on D-Bus
    v
[/xyz/openbmc_project/sensors/temperature/arduino_lm75]
    |
    | Exposed by OpenBMC sensor / Redfish stack
    v
[Redfish API]
    |
    | Verified by curl
    v
[/redfish/v1/Chassis/chassis/Sensors/...]
```
