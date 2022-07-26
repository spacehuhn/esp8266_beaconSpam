I've been playing with this, and making some big improvements to it. One of the bugs in the old version was that 50% of the time, it worked every time. **Now it's working 100% of every time.**

The big new feature is a new mode for assigning MAC addresses to SSIDs. The old mode is "0" ("macMode = 0"), and it works as expected, and can be seen in this start-up message:
```
//// Atom Smasher's Beacon Spammer v1.0 ////

// MACs:                 SSIDs:
     be:5d:3b:00:53:01     The Password is...
     be:5d:3b:00:53:02     Untrusted Network
     be:5d:3b:00:53:03     404 Network Unavailable
     be:5d:3b:00:53:04     The Internet
     be:5d:3b:00:53:05     Click Here for Wifi
     be:5d:3b:00:53:06     No Internet Access
     be:5d:3b:00:53:07     FBI Channel 90210
     be:5d:3b:00:53:08     Click Here to Download
     be:5d:3b:00:53:09     The Promised LAN
     be:5d:3b:00:53:0a     Free Public Wifi
     be:5d:3b:00:53:0b     $1 per hour
     be:5d:3b:00:53:0c     Russian Hackers
     be:5d:3b:00:53:0d     The LAN of the Free
     be:5d:3b:00:53:0e     No Connections Available
     be:5d:3b:00:53:0f     No More Mister Wifi
     be:5d:3b:00:53:10     Router? I Hardly Knew Her
     be:5d:3b:00:53:11     Connected, Secured
     be:5d:3b:00:53:12     The LAN Before Time
     be:5d:3b:00:53:13     Get off my LAN
     be:5d:3b:00:53:14     Silence of the LAN

// randomMacSeed:        SSIDs:                Started in:
     0x1234abcd            20                    615ms

Packets/s: 199.501
Packets/s: 200.000
Packets/s: 199.960
Packets/s: 199.960
Packets/s: 199.960
[...]
```

As expected, a random MAC is set, and (up to 255) SSIDs can sequentially increment the MAC address. The new mode "1" ("macMode = 1") sets the MAC addresses "randomly", as can be seen in this start-up message:
```
//// Atom Smasher's Beacon Spammer v1.0 ////

// MACs:                 SSIDs:
     be:5d:3b:00:53:9e     The Password is...
     ea:ff:b2:e4:2d:4e     Untrusted Network
     16:a1:29:c8:07:fe     404 Network Unavailable
     46:43:a0:ab:e2:ae     The Internet
     72:e5:17:8f:bc:5e     Click Here for Wifi
     9e:87:8e:72:97:0e     No Internet Access
     ce:29:05:56:71:be     FBI Channel 90210
     fa:cb:7c:39:4b:6e     Click Here to Download
     26:6d:f3:1d:26:1e     The Promised LAN
     56:0f:6a:01:00:ce     Free Public Wifi
     82:b1:e1:e4:da:7e     $1 per hour
     ae:53:57:c8:b5:2e     Russian Hackers
     de:f5:ce:ab:8f:df     The LAN of the Free
     0a:97:45:8f:69:8f     No Connections Available
     36:39:bc:72:44:3f     No More Mister Wifi
     66:db:33:56:1e:ef     Router? I Hardly Knew Her
     92:7d:aa:3a:f8:9f     Connected, Secured
     be:1f:21:1d:d3:4f     The LAN Before Time
     ee:c1:98:01:ad:ff     Get off my LAN
     1a:63:0f:e4:87:af     Silence of the LAN

// randomMacSeed:        SSIDs:                Started in:
     0x1234abcd            20                    615ms

Packets/s: 199.501
Packets/s: 199.900
Packets/s: 199.980
Packets/s: 199.920
Packets/s: 199.940
[...]
```

Note that the "randomMacSeed" is the same in both of those, and the first MAC address in the second start-up message resembles all of the MAC addresses in the first start-up message.

The "randomMacSeed" can be set randomly, giving different MAC addresses (per list of SSIDs names) every time it's re-started, or it can be set manually for consistency, or to "synchronise" two or more devices to have the same MAC addresses.

The code is also written to make it easy to manually set any parts of the MAC address, eg "set" the first three octets, or set the last four octets to "deadbeef", or whatever. The range of each octet's random assignment can also be set, and bitmasking can be used to exclude or set certain digits to certain values.

The start-up messages sent to the serial port seem useful, at least to me.

The biggest bug-fix was dealing with the "I/G bit"; now it works 100% of the time, every time.

The throttle function prevents it from sending more than 10 beacons per second, per SSID, per channel. As can be seen in the start-up messages, 20 SSIDs, on one channel, and it's being throttled to 200 packets/second. In my testing, it tops out at about 930 packets/second, so just by 100 SSDIS per channel, it will start to go slower than 10 beacons per second.

Some bug-fixes, some new features, some code clean-up. I'm keeping with the spirit of the original author, and not releasing any binaries. The project BEGS to be customised. Using the default SSID names is fine for testing, but lame for anything else. This was my first Arduino project and my first non-trivial programming in C, or whatever variant of C this is. Hopefully other will continue to build on it, and to learn from it.

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
