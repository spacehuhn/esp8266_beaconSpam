/*
  ===========================================
  (c) 2023, Atom Smasher
  https://github.com/atom-smasher/esp8266_beaconSpam

  based on the work of Stefan Kremser:
  https://github.com/spacehuhn/esp8266_beaconSpam

  I fixed fix some significant bugs and added some features, but I'm grateful to Stefan
  Kremser, as this was my first Arduino project, and my first real fight with C

  ===========================================
*/

//////// Settings ////////

// *** SSIDs (that's why you're here)
// *** max 32 characters per SSID. ok, technically it's 32 "octets", eg 32 ASCII characters or 32 bytes
// *** remember to use quotes and commas
const char ssidList[][33] PROGMEM = {

//  "12345678901234567890123456789012" // as a point of reference, this is 32 ASCII characters

    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine",
    "ten",
    "eleven",
    "twelve",
    "thirteen",
    "fourteen",
    "fifteen"

};

//////// channels ////////
// most parts of the world only use channels 1-13
// north america only uses channels 1-11
// add multiple channels like this: "{1, 2, 5}"
const uint8_t channels[] = {1}; // used Wi-Fi channels (available: 1-14)

//////// macMode ////////
// macMode:
// macMode = 0 : "classic" mac assignment; a random mac address is generated,
//   and subsequent mac addresses are sequentially incremented
// macMode = 1 : "mayhem" mac assignment; each SSID is assigned a random mac
const uint8_t macMode = 0;

//////// WPA2 ////////
// advertise OPEN or WPA2/AES
const bool wpa2 = 0;

//////// random seed for mac addresses ////////
// random seed for mac addresses. this can be set manually within the
// bounds of uint32, to consistently reproduce "random" mac adresses.
// leaving this as random produces different mac addresses on every restart.
// setting to manual allows mac addresses to be maintained across restarts, and
// can be used to "synchronise" two or more devices.
// seed is printed to serial port at start-up.
// humans are really bad at creating random numbers. a great way to manually make
// a random seed is using the "randoms" script - https://github.com/atom-smasher/randoms
// run that script like this: randoms -X 8
// or, on most modern linux systems, run this:
//   crc32 /proc/sys/kernel/random/uuid | awk '{print "0x"$0}'
// or this:
//   cut -c 1-8 < /proc/sys/kernel/random/uuid | awk '{print "0x"$0}'
uint32_t randomMacSeed = os_random();     // random seed on startup
//uint32_t randomMacSeed = 0x12345abc ;   // fixed seed; make it your own

//////// report interval ////////
// how many seconds should there be between reports sent to serial port?
// 0 = no reports sent to serial port
// keep it less than 2147483646
// intiger, seconds
uint32_t reportTime = 60;

//////// periodicaly re-key (randomise) the mac-addresses ////////
// intiger, milliseconds
// 0 = don't re-key
// 60000 = re-key every minute
// 300000 = re-key every 5 minutes
// 600000 = re-key every 10 minutes
// 1800000 = re-key every 30 minutes
// 3600000 = re-key every hour
// 86400000 = re-key every 24 hours
// 604800000 = re-key every week
// keep this number smaller than 2147483646
uint32_t rekeyTime = 0;

// if the rekeyTime is < 0
// this will either use the PRNG to deterministically re-key the mac addresses,
// or use os_random() to randomly re-key the mac addresses
// might be handy for keeping multiple devices in sync as mac addresses change
const bool rekeyPRNG = false;


//////// ALL "NORMAL" CONFIG OPTIONS ARE ABOVE THIS LINE ////////
//////// *********************************************** ////////

//////// Includes ////////
#include <ESP8266WiFi.h>

//extern "C" {
//#include "user_interface.h"
//  typedef void (*freedom_outside_cb_t)(uint8 status);
//  int wifi_register_send_pkt_freedom_cb(freedom_outside_cb_t cb);
//  void wifi_unregister_send_pkt_freedom_cb(void);
//  int wifi_send_pkt_freedom(uint8 *buf, int len, bool sys_seq);
//}

/////////////////////
// run-time variables
uint16_t channelIndex = 0;
uint8_t  macAddr[5];
uint8_t  macAddr_b[5];
uint8_t  wifi_channel;
uint32_t packetSize = 0;
uint32_t loopStartTime = 0;
uint32_t packetCounter = 0;
uint32_t packetRateTime = 0;
uint32_t packetRateTime_tmp = 0;
uint16_t ssidCount = 0;
uint32_t i = 0;
uint32_t ssidNum = 0;
uint32_t millisRollover = 0;
uint32_t microsRollover = 0;
uint32_t reportTime_fixed;
uint32_t rekeyTime_fixed = rekeyTime;
uint32_t sendPacketsLoopTime = 0;

// this is the delay that prevents the send-packet loop from running unnecessarily fast
// (1000 / sizeof(channels)) is a very sane number, here
uint32_t throttle_loop = (100000 / sizeof(channels));

//////////////////////////
// beacon frame definition
uint8_t beaconPacket[109] = {
  /*  0 - 3  */ 0x80, 0x00, 0x00, 0x00,             // Type/Subtype: managment beacon frame
  /*  4 - 9  */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Destination: broadcast
  /* 10 - 15 */ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // Source "mac"
  /* 16 - 21 */ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // Source "mac"

  // Fixed parameters
  /* 22 - 23 */ 0x00, 0x00,                         // Fragment & sequence number (will be done by the SDK)
  /* 24 - 31 */ 0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00, // Timestamp
  /* 32 - 33 */ 0xe8, 0x03,                         // "Beacon Interval"
                                                    // this does not affect the actual rate that beacons are sent
                                                    // 0x64, 0x00 => about every 100ms - 0xe8, 0x03 => about every 1s
  /* 34 - 35 */ 0x31, 0x00,                         // capabilities Tnformation

  // Tagged parameters

  // SSID parameters
  /* 36 - 37 */ 0x00, 0x20,                         // Tag: Set SSID length, Tag length: 32
  /* 38 - 69 */ 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20,                           // SSID

  // Supported Rates
  /* 70 - 71 */ 0x01, 0x08,                         // Tag: Supported Rates, Tag length: 8
  /* 72 */ 0x82,                    // 1(B)
  /* 73 */ 0x84,                    // 2(B)
  /* 74 */ 0x8b,                    // 5.5(B)
  /* 75 */ 0x96,                    // 11(B)
  /* 76 */ 0x24,                    // 18
  /* 77 */ 0x30,                    // 24
  /* 78 */ 0x48,                    // 36
  /* 79 */ 0x6c,                    // 54

  // Current Channel
  /* 80 - 81 */ 0x03, 0x01,         // Channel set, length
  /* 82 */      0x01,               // Current Channel

  // RSN information
  /*  83 -  84 */ 0x30, 0x18,
  /*  85 -  86 */ 0x01, 0x00,
  /*  87 -  90 */ 0x00, 0x0f, 0xac, 0x02,
  /*  91 -  92 */ 0x02, 0x00,
  /*  93 - 100 */ 0x00, 0x0f, 0xac, 0x04, 0x00, 0x0f, 0xac, 0x04,
  /* 101 - 102 */ 0x01, 0x00,
  /* 103 - 106 */ 0x00, 0x0f, 0xac, 0x02,
  /* 107 - 108 */ 0x00, 0x00
};

// Shift out channels one by one
void nextChannel() {
  if (sizeof(channels) > 1) {
    channelIndex++;
    if (channelIndex >= sizeof(channels)) {
      channelIndex = 0;
    }
    uint8_t ch = channels[channelIndex];
    if (ch != wifi_channel && ch >= 1 && ch <= 14) {
      wifi_channel = channels[channelIndex];
      wifi_set_channel(wifi_channel);
    }
  }
}

// Random MAC generator
void randomMac() {
  // sure, this could be done as a simple loop, but formatting it like
  // this makes it easy to "manually" set all or part of the mac addresses,
  // either fixed or within a given range of random values
  //
  // THE LOWEST TWO BITS OF THE FIRST OCTET ARE IMPORTANT!
  // aside from just picking a number at random, use bitwise operations to set them as needed:
  // "& 0xfe" : this ensures the I/G bit is "0" (unicast), which is kind of necessary for spoofing an AP.
  //            the U/L bit is left to chance.
  // "& 0xfc" : this ensures both the I/G bit and the U/L bit are "0" (unicast, universal).
  // "& 0xfe | 0x02" : this ensures the I/G bit is "0", and the U/L bit is "1" (unicast, local).
  randomSeed(uint32_t(randomMacSeed));
  macAddr[0] = uint8_t(random(0x0, 0x100)) & 0xfe | 0x02 ; // use bitwise oparations to properly set the least significant bits
  macAddr[1] = uint8_t(random(0x0, 0x100));
  macAddr[2] = uint8_t(random(0x0, 0x100));
  macAddr[3] = uint8_t(random(0x0, 0x100));
  macAddr[4] = uint8_t(random(0x0, 0x100));
  macAddr[5] = uint8_t(0x00); // this one gets assigned sequentially,
                              // later on, when this mode is in use
}

void mayhemMac() {
  // SEE COMMENTS, ABOVE
  macAddr[0] = uint8_t(random(0x0, 0x100)) & 0xfe | 0x02 ; // SEE COMMENTS, ABOVE
  macAddr[1] = uint8_t(random(0x0, 0x100));
  macAddr[2] = uint8_t(random(0x0, 0x100));
  macAddr[3] = uint8_t(random(0x0, 0x100));
  macAddr[4] = uint8_t(random(0x0, 0x100));
  macAddr[5] = uint8_t(random(0x0, 0x100));
}

///////////////////////////////
void displayMacsSsids() {
  // mac and ssid startup message
  Serial.println("// MACs:                 SSIDs:");
  ssidCount = sizeof(ssidList) / sizeof(ssidList[0]);
  i = 0;
  if (0 == macMode) {
    // start macMode=0
    randomMac();
    for (i = 0; i < ssidCount; i++) {
      yield(); // needed for extra-large lists
      Serial.printf("     %02x:%02x:%02x:%02x:%02x:%02x     %s\n",
        macAddr[0],
        macAddr[1],
        macAddr[2],
        uint8_t(macAddr[3] + ((macAddr[4] + (i / 0x100)) / 0x100)), // rollover mac address for large ssid lists
        uint8_t(macAddr[4] + (i / 0x100)),                          // rollover mac address for large ssid lists
        uint8_t(i), // "i" bound by uint8 is effectively "i % 0x100", and it becomes "macAddr[5]"
        ssidList[i]);
        // end start macMode=0
    }
  } else {
    // start macMode=1
    randomSeed(uint32_t(randomMacSeed));
    for (i = 0; i < ssidCount; i++) {
      yield(); // needed for extra-large lists
      mayhemMac();
      Serial.printf("     %02x:%02x:%02x:%02x:%02x:%02x     %s\n",
        macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5],
        ssidList[i]);
        // end macMode=1
    }
  }
}


void setup() {

  // start serial
  Serial.begin(115200);
  //Initialize serial and wait for port to open:
  while (!Serial) {
    delay(10);
  }
  // wait for serial port to connect
  delay(300);
  Serial.println();


  // set packetSize
  packetSize = sizeof(beaconPacket);
  if (wpa2) {
    beaconPacket[34] = 0x31;
  } else {
    beaconPacket[34] = 0x21;
    packetSize -= 26;
  }


  // start WiFi
  //WiFi.mode(WIFI_AP_STA);
  wifi_set_opmode(WIFI_OFF);
  delay(1);
  wifi_set_opmode(STATION_MODE);
  delay(1);


  // Set to default WiFi channel
  wifi_channel = channels[0];
  wifi_set_channel(channels[0]);
  delay(1);


  ///////////////////////////////
  // mac and ssid startup message
  Serial.println("\n//// Atom Smasher's Beacon Spammer v1.3d ////\n");
  displayMacsSsids();


  // print misc startup info
  Serial.printf("\n// randomMacSeed:        SSIDs:                Started in:\n     0x%08x            %-21d %4.3fs\n\n",
    uint32_t(randomMacSeed), ssidCount, (uint32_t(millis()) * 0.001 ));


  // for convenience, reportTime is configured in seconds, but it's processed in milli-seconds
  reportTime *= 1000;
  reportTime_fixed = reportTime;


  macAddr_b[3] = macAddr[3]; // rollover safety
  macAddr_b[4] = macAddr[4]; // rollover safety

}

void loop() {

  if ((int32_t(micros() - sendPacketsLoopTime)) >= 0 ) {

    nextChannel();

    ssidNum = 0;

    if (1 == macMode) {
      randomSeed(uint32_t(randomMacSeed));
    }

    // for each ssid ...
    for (i = 0; i < ssidCount; i++) {

        ///////////////
        // if mayhemMac
        if (1 == macMode) {
          mayhemMac();
        } else {
          // classic mac mode
          macAddr[3] = uint8_t(macAddr_b[3] + ((macAddr_b[4] + (ssidNum / 0x100)) / 0x100)); // gracefully handle >256 SSIDs
          macAddr[4] = uint8_t(macAddr_b[4] + (ssidNum / 0x100));                            // gracefully handle >256 SSIDs
          macAddr[5] = uint8_t(ssidNum);
        }

        ssidNum++;

        // write MAC address into beacon frame
        memcpy(&beaconPacket[10], macAddr, 6);
        memcpy(&beaconPacket[16], macAddr, 6);

        // write new SSID into beacon frame
        memcpy_P(&beaconPacket[38], &ssidList[i], 0x20);

        // set channel for beacon frame
        beaconPacket[82] = wifi_channel;

        // this while-loop ensures that beacons are actually being sent
        // and it's a lot slower than not ensuring that beacons are actually being sent.
        // to maintain 10 beacons per ssid per second per channel, the ssid list should be limited
        // to about 70 ssids with wpa, or about 80 ssids without wpa; that's assuming only one channel is used
        // larger lists of ssids will result in fewer than 10 beacons per ssid per second per channel.
        // this really is the speed-bump in the whole process.
        while (0 != wifi_send_pkt_freedom(beaconPacket, packetSize, 0)) {
          yield();
        }

        // packet sent
        packetCounter += 1;
      }

      sendPacketsLoopTime = sendPacketsLoopTime + throttle_loop;
      if ((int32_t(micros() - sendPacketsLoopTime)) >= 0 ) {
        sendPacketsLoopTime = micros();
      }
  }

    // rolover notices
    // micros() rolls over every           1h:11:34.967295
    // millis() rolls over every 49 days, 17h:02:47.295
    if (micros() < microsRollover) {
      Serial.printf("[%.3f] *** micros() clock rollover\n", (millis() * 0.001));
    }
    microsRollover = micros();
    //
    if (millis() < millisRollover) {
      Serial.printf("[%.3f] ****** millis() clock rollover\n", (millis() * 0.001));
    }
    millisRollover = millis();

    // re-key ssids, if needed
    if (0 < rekeyTime) {
      if ((int32_t(millis() - rekeyTime)) >= 0) {
        if (rekeyPRNG) {
          // this may look "random", but it's very deterministic, and thus repeatable
          randomMacSeed = (random() + random());
          Serial.printf("[%.3f] New randomMacSeed: 0x%08x (PRNG)\n", // display the new randomMacSeed
            (millis() * 0.001),
            uint32_t(randomMacSeed));
        } else {
          // this is effectively "random" in the sense that it's non-deterministic
          randomMacSeed = os_random();
          Serial.printf("[%.3f] New randomMacSeed: 0x%08x (os_random)\n", // display the new randomMacSeed
            (millis() * 0.001),
            uint32_t(randomMacSeed));
        }
        if (0 == macMode) {
          // rekey for classic mac-mode
          randomMac();
          macAddr_b[3] = macAddr[3]; // rollover safety for <256 ssids
          macAddr_b[4] = macAddr[4]; // rollover safety for <256 ssids
        }
        // with absurdly huge ssid lists, it may be best to comment out the next line,
        // and NOT display them at a rate that's limited by the serial port speed
        displayMacsSsids(); // display all new ssids and their new mac addresses

        rekeyTime += rekeyTime_fixed;
        if ((int32_t(millis() - rekeyTime)) >= 0 ) {
          rekeyTime = millis();
        }
      }
    }

    // show packet-rate every n seconds
    // if this gets delayed by a huge ssid list, or something, it will still report accurate numbers
    packetRateTime_tmp = millis();
    if (0 < reportTime) {
      if ((int32_t(millis() - reportTime)) >= 0 ) {
        // the percent display here is percent of full speed; that's 10 beacons per second, per ssid, per channel
        Serial.printf("[%.3f] %.2f packets/s, %5.1f%%\n",
          (millis() * 0.001),
          (float((packetCounter * 1000.0) / int32_t(packetRateTime_tmp - packetRateTime) )),
          (float((packetCounter * 10000.0) / int32_t(packetRateTime_tmp - packetRateTime) ) / ssidCount / (sizeof(channels))));
        packetRateTime = packetRateTime_tmp;
        packetCounter = 0;
        reportTime += reportTime_fixed;
        if ((int32_t(millis() - reportTime)) >= 0 ) {
          reportTime = millis();
        }
        // this line, if enabled, shows the current time (millis()) in HEX and DEC, and the time the next report is due, in HEX and DEC
        //Serial.printf("         reportTime: 0x%08x, %010u --> 0x%08x, %010u\n", millis(), millis(),  reportTime, reportTime);
      }
    }

}
