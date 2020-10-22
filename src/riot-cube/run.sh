#!/bin/sh

export PATH=/home/roberto/esp/xtensa-esp8266-elf/bin:$PATH
export ESP8266_RTOS_SDK_DIR=/home/roberto/esp/ESP8266_RTOS_SDK

make BOARD=esp8266-esp-12x flash term 
