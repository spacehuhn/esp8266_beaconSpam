@echo off
:: a DOS command-line to flash the binary without recompiling

ESP8266_PORT=COM15

C:\Users\%USERNAME%\AppData\Local\Arduino15\packages\esp8266\tools\python3\3.7.2-post1/python3 C:\Users\%USERNAME%\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\2.7.4/tools/upload.py --chip esp8266 --port %ESP8266_PORT% --baud 115200 --before default_reset --after hard_reset write_flash 0x0 C:\Users\%USERNAME%\AppData\Local\Temp\VMBuilds\esp8266_beaconSpam\esp8266_nodemcuv2\Release/esp8266_beaconSpam.ino.bin
