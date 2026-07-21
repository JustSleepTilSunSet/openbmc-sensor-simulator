# Environment Stack

| Component               |  Required | Purpose                                         |
| ----------------------- | --------: | ----------------------------------------------- |
| Docker+unison           |  Optional | Sync virtual machine files between host and vm. |
| VirtualBox              |  Optional | Isolated packages environment                   |
| OpenBMC QEMU            | Must-have | Runtime target                                  |
| LM75 temperature sensor | Must-have | Temperature sensor                              |

# Tech Stack

- C++

# Package stack

1. [arduino-cli](https://github.com/arduino/arduino-cli)
2. [OpenBMC](https://github.com/openbmc/openbmc)
