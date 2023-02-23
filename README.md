# Device library for controlling the Ostentus faceplate

Ostentus is a PCB faceplate for Golioth reference designs that has an ePaper display, indicator LEDs, and
capacitive touch buttons. This library allows Zephyr projects to control
Ostentus via an i2c connection.

## Zephyr requirements

1. `CONFIG_I2C` enabled in Kconfig
2. `click-i2c` alias to the correct I2Cx bus in Devicetree
3. Add `libostentus.c` to CMakeLists.txt
4. Include `libostentus.h` in files that need to issue Ostentus commands

