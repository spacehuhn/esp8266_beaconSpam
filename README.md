# ESP32 Beacon Spam


## Changes

I transplant this code at 2018.09.25
- Transplant to based on ESP32

I didn't publish a .bin file on purpose. This project is easy to create with Arduino and everyone should be able to get it working!  
The goal of this is to motivate people to get into hacking, electronics and Arduino, so don't be lazy and start making your own stuff! :D

## About

![network scanner](https://raw.githubusercontent.com/spacehuhn/esp8266_beaconSpam/master/img/networkscanner.jpg)

The Arduino sketch comes with 50 default SSID names, but you can edit that list easily in the source code :).  
By constantly broadcasting the so called beacon frames, your standard WiFi scanner will think there are active networks nearby and adds them to the list.  
In reality though, it is just advertising these network names without actually creating them. So there is no way you could connect to one of the "created" networks.  

It is using the `esp_wifi_80211_tx` function in the ESP32 Arduino Core SDK. This function allows packet injection for specific Wi-Fi frames.  

## Disclaimer

Even if it is more of a fun party trick than something that could actually do any harm, **please use it respectively!**
Some people might interpret that as a "Jammer" and those are illegal. But this porject here is just sending a bunch of WiFi packets through the air and works within the 802.11 Wi-Fi standard.

## Installation

- First get an ESP32 development board! 
- Install [Arduino](https://www.arduino.cc/en/Main/software)
- Install the [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md) 
- Clone this project
- Edit the SSIDs if you want
- Select the correct Board under Tools > Board, the serial (COM) port your device is using and hit upload!


## License

This project is licensed under the MIT License - see the [license file](LICENSE) file for details

## Sources
 
The 50 default SSIDs are from: http://www.makeuseof.com/tag/50-funny-wi-fi-names-network-ssid/  
More infos about beacon frames: https://mrncciew.com/2014/10/08/802-11-mgmt-beacon-frame/  
