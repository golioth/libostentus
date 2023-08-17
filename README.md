# Device library for controlling the Ostentus faceplate

![Golioth Ostentus faceplate](img/golioth-ostentus-faceplate.jpg)

Ostentus is a PCB faceplate for Golioth reference designs that has an ePaper
display, indicator LEDs, and capacitive touch buttons. This library allows
Zephyr projects to control Ostentus via an i2c connection.

Note: Ostentus is a completely optional element in Golioth Reference Designs

## Zephyr requirements

1. `CONFIG_I2C` enabled in Kconfig
2. `CONFIG_LIB_OSTENTUS` enabled in Kconfig
3. `click-i2c` alias to the correct I2Cx bus in Devicetree
4. Include `libostentus.h` in files that need to issue Ostentus commands

