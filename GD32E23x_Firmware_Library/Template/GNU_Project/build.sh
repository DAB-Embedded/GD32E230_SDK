#!/bin/sh
make clean
make GCC_PATH=/home/user/tools/gcc-arm-none-eabi-10.3-2021.10/bin DEBUG_BUILD=1
/home/user/sources/training/gd32f470-firmware-pro/openocd/openocd -s "/home/user/sources/training/gd32f470-firmware-pro/openocd/tcl" -f interface/ftdi/minimodule-swd.cfg -f target/gd32e23x.cfg -c "adapter speed 2000" -c "program build/Template.elf" -c "reset run" -c "exit"
