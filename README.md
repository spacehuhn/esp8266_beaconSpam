I've been playing with this, and making some big improvements to it. One of the bugs in the older version was that 50% of the time, it worked every time. **Now it's working 100% of every time.**

The big new feature is a new mode for assigning MAC addresses to SSIDs. The old mode is "0" ("macMode = 0"), and it works as expected, and can be seen in this start-up message:
```
//// Atom Smasher's Beacon Spammer v1.1 ////

// MACs:                 SSIDs:

     7e:1c:b7:8d:80:00     The Password is...
     7e:1c:b7:8d:80:01     Untrusted Network
     7e:1c:b7:8d:80:02     404 Network Unavailable
     7e:1c:b7:8d:80:03     The Internet
     7e:1c:b7:8d:80:04     Click Here for Wifi
     7e:1c:b7:8d:80:05     No Internet Access
     7e:1c:b7:8d:80:06     FBI Channel 90210
     7e:1c:b7:8d:80:07     Click Here to Download
     7e:1c:b7:8d:80:08     The Promised LAN
     7e:1c:b7:8d:80:09     Free Public Wifi
     7e:1c:b7:8d:80:0a     $1 per hour
     7e:1c:b7:8d:80:0b     Russian Hackers
     7e:1c:b7:8d:80:0c     The LAN of the Free
     7e:1c:b7:8d:80:0d     No Connections Available
     7e:1c:b7:8d:80:0e     No More Mister Wifi
     7e:1c:b7:8d:80:0f     Router? I Hardly Knew Her
     7e:1c:b7:8d:80:10     Connected, Secured
     7e:1c:b7:8d:80:11     The LAN Before Time
     7e:1c:b7:8d:80:12     Get off my LAN
     7e:1c:b7:8d:80:13     Silence of the LAN

// randomMacSeed:        SSIDs:                Started in:
     0x12345abc            20                    615ms

Packets/s: 199.561
Packets/s: 200.000
Packets/s: 199.940
Packets/s: 199.920
Packets/s: 200.000
[...]
```

As expected, a random MAC is set, and SSIDs will sequentially increment the MAC address. As of v1.1, this works with a little over 23,000 SSID names, rolling over the 4th and 5th octets of the MAC address as needed; more SSID names made compilation fail. The new mode "1" ("macMode = 1") sets the MAC addresses "randomly", as can be seen in this start-up message:
```
//// Atom Smasher's Beacon Spammer v1.1 ////

// MACs:                 SSIDs:

     7e:1c:b7:8d:80:6d     The Password is...
     a2:b9:c6:2e:0d:6a     Untrusted Network
     02:7e:32:0a:96:c0     404 Network Unavailable
     da:6f:e4:dc:c2:7e     The Internet
     9e:8f:7f:08:33:d8     Click Here for Wifi
     4a:9b:4d:6b:19:74     No Internet Access
     32:d8:17:5f:aa:24     FBI Channel 90210
     ce:94:0a:e0:b0:aa     Click Here to Download
     72:f9:39:ab:21:55     The Promised LAN
     de:e0:a2:b0:be:af     Free Public Wifi
     6a:81:1b:a4:e9:3b     $1 per hour
     e6:87:c9:21:fc:6e     Russian Hackers
     ea:8e:43:37:7e:03     The LAN of the Free
     56:71:fb:17:a5:e8     No Connections Available
     12:75:37:05:0d:e6     No More Mister Wifi
     3e:51:41:12:16:88     Router? I Hardly Knew Her
     c2:8f:4e:e0:fc:2e     Connected, Secured
     8a:70:da:47:55:b8     The LAN Before Time
     4e:eb:ab:09:ae:b9     Get off my LAN
     ea:51:cc:b8:d0:05     Silence of the LAN

// randomMacSeed:        SSIDs:                Started in:
     0x12345abc            20                    619ms

Packets/s: 199.541
Packets/s: 200.000
Packets/s: 199.920
Packets/s: 200.000
Packets/s: 199.940
[...]
```

Note that the "randomMacSeed" is the same in both of those, which is why the first MAC address in the second start-up message resembles all of the MAC addresses in the first start-up message.

The "randomMacSeed" can be set randomly, giving different MAC addresses (per list of SSIDs names) every time it's re-started, or it can be set manually for consistency, or to "synchronise" two or more devices to have the same MAC addresses.

The code is also written to make it easy to manually set any parts of the MAC address, eg "set" the first three octets, or set the last four octets to "deadbeef", or whatever. The range of each octet's random assignment can also be set, and bitmasking can be used to exclude or set certain digits to certain values.

The start-up messages sent to the serial port seem useful, at least to me.

The biggest bug-fix that was inherited from earlier versions was fixing the "I/G bit". Left to chance, 50% of the time it works every time. Now it works 100% of the time, every time. While I was fixing that, I coded to set the "U/L bit". The default setting leaves that set to "local" mode, but that's easy to change as desired, or leave it to random chance.

The throttle function prevents it from sending more than 10 beacons per second, per SSID, per channel. As can be seen in the start-up messages, 20 SSIDs, on one channel, and it's being throttled to 200 packets/second. In my testing, it tops out at about 930 packets/second, so at about 100 SSIDs per channel, it will start to go slower than 10 beacons per second; this may vary with different hardware. 

Some bug-fixes, some new features, some code clean-up. I'm keeping with the spirit of the original author, and not releasing any binaries. The project BEGS to be customised. Using the default SSID names is fine for testing, but lame for anything else. This was my first Arduino project and my first non-trivial programming in C, or whatever variant of C this is. Hopefully others will continue to build on it, and to learn from it.

By the pevious author:

# ESP8266 Beacon Spam


<p align="center"><img alt="project-logo" width="300" src="https://raw.githubusercontent.com/spacehuhn/esp8266_beaconSpam/master/img/beacon_spam.png"></p>
 
<p align="center">

<br>
p<b>Advertise thousdands of WiFi access points with custom SSIDs.<br>
<br>

ESP32 version by [@Tnze](https://github.com/Tnze): [esp32_beaconSpam](https://github.com/Tnze/esp32_beaconSpam).

I didn't publish a .bin file on purpose. This project is easy to create with Arduino and everyone should be able to get it working!  
The goal of this is to motivate people to get into hacking, electronics and Arduino, so don't be lazy and start making your own stuff! :D

## About

![network scanner](https://raw.githubusercontent.com/spacehuhn/esp8266_beaconSpam/master/img/networkscanner.jpg)

This project emerged from the [ESP8266 Deauther](https://github.com/spacehuhn/esp8266_deauther) that also has this type of attack implemented.  
But while the Deauther is very complex and might seem intimidating to an Arduino beginner, this project is simple, hackable and easy get running!  

The Arduino sketch comes with 50 default SSID names, but you can edit that list easily in the source code :).  
By constantly broadcasting the so-called beacon frames, your standard WiFi scanner will think there are active networks nearby and adds them to the list.  
In reality though, it is just advertising these network names without actually creating them. So there is no way you could connect to one of the "created" networks.  

It is using the `wifi_send_pkt_freedom` function in the ESP8266 Arduino Core SDK. This function allows packet injection for specific Wi-Fi frames.  

## Disclaimer

Even if it is more of a fun party trick than something that could actually do any harm, **please use it respectfully!**
Some people might interpret this as a "Jammer" and those are illegal. But this project is just sending a bunch of WiFi packets through the air and works within the 802.11 Wi-Fi standard.

## Installation

- First get an ESP8266 development board! If you don't know which one, have a look at the [supported devices](https://github.com/spacehuhn/esp8266_deauther/wiki/Supported-Devices) page from the Deauther wiki. It's for a different project, but the requirements are the same.
- Install [Arduino](https://www.arduino.cc/en/Main/software)
- Install the [ESP8266 Arduino Core](https://github.com/esp8266/Arduino#installing-with-boards-manager) version 2.0.0 or newer (no more changes are required)
- Download [this project](https://github.com/spacehuhn/esp8266_beaconSpam/archive/master.zip)
- Extract the .zip file somewhere and open the `esp8266_beaconSpam/esp8266_beaconSpam.ino` file with Arduino
- Edit the SSIDs if you want
- Select the correct Board under Tools > Board, the serial (COM) port your device is using and hit upload!

If you have trouble uploading, have a look at the [installation guide](https://github.com/spacehuhn/esp8266_deauther/wiki/Installation#drivers-and-com-port) on the Deauther Wiki. There you will also find drivers for the most common USB-Serial chips.  
Again, this is for the Deauther and not for this project, but the hardware (ESP8266) is the same and you will find a lot of trouble-shooting info on the Wiki there! :)  

Additionally, here's a video tutorial :)  
[![$5 WiFi Spammer](https://img.youtube.com/vi/Zq7QNpPxCqE/0.jpg)](https://www.youtube.com/watch?v=Zq7QNpPxCqE)

## License

This project is licensed under the MIT License - see the [license file](LICENSE) file for details

## Sources
 
The 50 default SSIDs are from: http://www.makeuseof.com/tag/50-funny-wi-fi-names-network-ssid/  
More info about beacon frames: https://mrncciew.com/2014/10/08/802-11-mgmt-beacon-frame/  
