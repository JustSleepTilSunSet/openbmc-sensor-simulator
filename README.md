# OpenBMC Mock Sensor

This project integerated arduino application into OpenBMC QEMU D-Bus and optionally exposes it through Redfish.

# Project Goal

1. Verify the sensor through D-Bus and optionally Redfish.
2. Package the C++ service into an OpenBMC image.

# Architecture

- See [arichecture](./docs/architecture.md)

# Tech/Environment Stack

- See [Tech/Env stack](./docs/packages_stack.md)

# Repository Structure

```
.
├── Dockerfile
├── compose.yaml
├── docs
├── mock-sensor-recipe
│   ├── conf
│   ├── recipes-demo
│   ├── recipes-mock-sensor
│   └── recipes-phosphor
│       └── images
│           └── obmc-phosphor-image.bbappend
├── scripts
│   ├── startup
│   │   ├── build_sdbusplus.sh
│   │   ├── lib_startup.sh
│   │   ├── run_sdbusplus.sh
│   │   └── startup_bmc.sh
│   └── verify
│       └── verify_sdbusplus.sh
└── temperature-sensor
    ├── README.md
    ├── i2c_scanner
    │   └── i2c_scanner.ino
    ├── led_flow
    └── setup.sh
```

## Quick start

- See [Development Setup](./docs/development_setup.md)

## Verification

- See [verification](./docs/verification.md)

## Troubleshooting

- See [Troubleshooting / FAQ](./docs/FAQ.md)
