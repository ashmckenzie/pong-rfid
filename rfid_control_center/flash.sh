#!/bin/bash

cd ~/src/personal/pong-rfid/core/core-firmware/build
make && dfu-util -d 1d50:607f -a 0 -s 0x08005000:leave -D core-firmware.bin core-firmware.elf
