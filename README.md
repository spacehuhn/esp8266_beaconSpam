# ESP8266 Beacon Spam


<p align="center"><img alt="project-logo" width="300" src="https://raw.githubusercontent.com/spacehuhn/esp8266_beaconSpam/master/img/beacon_spam.png"></p>
 
<p align="center">
🐦 <a href="https://twitter.com/spacehuhn">Twitter</a>
| 📺 <a href="https://www.youtube.com/channel/UCFmjA6dnjv-phqrFACyI8tw">YouTube</a>
| 🌍 <a href="https://spacehuhn.de">spacehuhn.de</a><br/>
<br>
<b>Advertise hundrets of WiFi access points with custom SSIDs.<br>
<br>
Support the development of this project by purchasing one of the <a href="https://github.com/spacehuhn/esp8266_deauther/wiki/Supported-Devices">official deauther boards</a>.<br/>Or become a patron on <a href="https://patreon.com/spacehuhn" target="_blank">patreon.com/spacehuhn</a>.</b>
</p>

ESP32 version by [@Tnze](https://github.com/Tnze): [esp32_beaconSpam](https://github.com/Tnze/esp32_beaconSpam).  

## Changes

I wrote the initial sketch in 2017 and it had some flaws.  
Here are the latest changes (06/2018):  
- Now works with ESP8266 Arduino Core v2.0.0 or newer without **any** further modification
- Fixed the bug that the SSIDs are not shown correctly on IOS
- No need for a SD card anymore to save hundrets of SSIDs, they are all stored in the PROGMEM now

Using the SD card might be useful for some people... but thanks to the magic of git, you can still view the old repository [here](https://github.com/spacehuhn/esp8266_beaconSpam/tree/v1.0). 

I didn't publish a .bin file on purpose. This project is easy to create with Arduino and everyone should be able to get it working!  
The goal of this is to motivate people to get into hacking, electronics and Arduino, so don't be lazy and start making your own stuff! :D

## About

![network scanner](https://raw.githubusercontent.com/spacehuhn/esp8266_beaconSpam/master/img/networkscanner.jpg)

This project emerged from the [ESP8266 Deauther](https://github.com/spacehuhn/esp8266_deauther) that also has this type of attack implemented.  
But while the Deauther is very complex and might seem intimidating to an Arduino beginner, this project is simple, hackable and easy get running!  

The Arduino sketch comes with 50 default SSID names, but you can edit that list easily in the source code :).  
By constantly broadcasting the so called beacon frames, your standard WiFi scanner will think there are active networks nearby and adds them to the list.  
In reality though, it is just advertising these network names without actually creating them. So there is no way you could connect to one of the "created" networks.  

It is using the `wifi_send_pkt_freedom` function in the ESP8266 Arduino Core SDK. This function allows packet injection for specific Wi-Fi frames.  

## Disclaimer

Even if it is more of a fun party trick than something that could actually do any harm, **please use it respectively!**
Some people might interpret that as a "Jammer" and those are illegal. But this porject here is just sending a bunch of WiFi packets through the air and works within the 802.11 Wi-Fi standard.

## Installation

- First get an ESP8266 development board! If you don't know which one, have a look at the [supported devices](https://github.com/spacehuhn/esp8266_deauther/wiki/Supported-Devices) page from the Deauther wiki. It's for a different project, but the requirements are the same.
- Install [Arduino](https://www.arduino.cc/en/Main/software)
- Install the [ESP8266 Arduino Core](https://github.com/esp8266/Arduino#installing-with-boards-manager) version 2.0.0 or newer (no more changes are required)
- Download [this project](https://github.com/spacehuhn/esp8266_beaconSpam/archive/master.zip)
- Extract the .zip file somewhere and open the `esp8266_beaconSpam/esp8266_beaconSpam.ino` file with Arduino
- Edit the SSIDs if you want
- Select the correct Board under Tools > Board, the serial (COM) port your device is using and hit upload!

If you have troubles uploading, have a look at the [installation guide](https://github.com/spacehuhn/esp8266_deauther/wiki/Installation#drivers-and-com-port) on the Deauther Wiki. There you will also find drivers for the most common USB-Serial chips.  
Again, this is for the Deauther and not for this project, but the hardware (ESP8266) is the same and you will find a lot of trouble-shooting infos on the Wiki there! :)  

Here's also a video tutorial :)  
[![$5 WiFi Spammer](https://img.youtube.com/vi/Zq7QNpPxCqE/0.jpg)](https://www.youtube.com/watch?v=Zq7QNpPxCqE)

## License

This project is licensed under the MIT License - see the [license file](LICENSE) file for details

## Sources
 
The 50 default SSIDs are from: http://www.makeuseof.com/tag/50-funny-wi-fi-names-network-ssid/  
More infos about beacon frames: https://mrncciew.com/2014/10/08/802-11-mgmt-beacon-frame/  
