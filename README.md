# esp8266_beaconSpam
Creates up to a thousand WiFi access points with custom SSIDs.  

**Support me and my projects on [Patreon!](https://www.patreon.com/spacehuhn)**  
[<img width="200" alt="Support me on Patreon" src="https://c5.patreon.com/external/logo/downloads_wordmark_navy.png">](https://www.patreon.com/spacehuhn)  


**Note:** This is a side project emerged from my [ESP8266 Deauther](https://github.com/spacehuhn/esp8266_deauther).  
Please have a look at this project first, the installation steps are the same.  

The ESP8266 will send 50 SSIDs by default.  
You can change the names in the code to whatever you prefer.  

**Optional:** You can use a SD card reader via the SPI pins (don't forget to set the chipSelect in the code).  
If connected it will load the SSIDs from `ssid.txt`.

## Disclaimer

Use it only for testing purposes on your own devices!

Please check the legal regulations in your country before using it. Jamming transmitters are illegal in most countries 
and this device can fall into the same category (even if it’s technically not the same).


## License

This project is licensed under the MIT License - see the [license file](LICENSE) file for details

## Sources
 
The 50 default SSIDs are from http://www.makeuseof.com/tag/50-funny-wi-fi-names-network-ssid/  
The ssid.txt is from https://wigle.net/stats#ssidstats  
