#include <ESP8266WiFi.h>

extern "C" {
  #include "user_interface.h"
}
#include <SPI.h>
#include <SD.h>

/* ================ SETTINGS ================ */
const int maxSize = 1000; // List size
const int maxLen = 32; // max. SSID length
const bool spamWPA = true; //spam WPA(2) protected networks
const bool randomChannel = true; // send on random channels
const int channel = 11;
const int chipSelect = D8; // for the SD card reader
const String fileName = "ssid.txt"; // name of the ssid file on the SD card
char ssids[maxSize][maxLen] = {"Mom Use This One","Abraham Linksys","Benjamin FrankLAN","Martin Router King","John Wilkes Bluetooth","Pretty Fly for a Wi-Fi","Bill Wi the Science Fi","I Believe Wi Can Fi","Tell My Wi-Fi Love Her","No More Mister Wi-Fi","LAN Solo","The LAN Before Time","Silence of the LANs","House LANister","Winternet Is Coming","Ping’s Landing","The Ping in the North","This LAN Is My LAN","Get Off My LAN","The Promised LAN","The LAN Down Under","FBI Surveillance Van 4","Area 51 Test Site","Drive-By Wi-Fi","Planet Express","Wu Tang LAN","Darude LANstorm","Never Gonna Give You Up","Hide Yo Kids, Hide Yo Wi-Fi","Loading…","Searching…","VIRUS.EXE","Virus-Infected Wi-Fi","Starbucks Wi-Fi","Text ###-#### for Password","Yell ____ for Password","The Password Is 1234","Free Public Wi-Fi","No Free Wi-Fi Here","Get Your Own Damn Wi-Fi","It Hurts When IP","Dora the Internet Explorer","404 Wi-Fi Unavailable","Porque-Fi","Titanic Syncing","Test Wi-Fi Please Ignore","Drop It Like It’s Hotspot","Life in the Fast LAN","The Creep Next Door","Ye Olde Internet"};
/* ========================================== */

File ssdiFile;

char ssidLengths[maxSize];
char macs[maxSize][6];
char channels[maxSize]; 

int ssidNum = 50;
String ssid;

int _packetSize = 0;
uint8_t _packet[110];

unsigned long previousMillis = 0;

uint8_t packetBegin[36] = { 
  0x80, 0x00, 
  0x00, 0x00, //beacon
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //destination: broadcast
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //source
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //source
  0xc0, 0x6c, 
  0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00, 
  0xe8, 0x03, 
  0x01, 0x04/*, 
  0x00, 0x06, //SSID size
  0x72, 0x72, 0x72, 0x72, 0x72, 0x72, //SSID
  >>beaconPacket_end<<*/
};

uint8_t packetEnd[13] = {
  0x01, 0x08, 0x82, 0x84,
  0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03, 0x01,
  0x03 //channel
};

uint8_t wpa2tag[26] = {
  0x30, //RSN tag
  0x18, //tag length
  0x01, 0x00, //RSN version
  0x00, 0x0f, 0xac, 0x02, //cipher (TKIP)
  0x02, 0x00, //pair cipher
  0x00, 0x0f, 0xac, 0x04, //cipher (AES)
  0x00, 0x0f, 0xac, 0x02, //cipher (TKIP)
  0x01, 0x00, //AKM count
  0x00, 0x0f, 0xac, 0x02, //PSK
  0x00, 0x00 //RSN capabilities
};

//source: https://forum.arduino.cc/index.php?topic=38107.0
void PrintHex8(uint8_t *data, uint8_t length){
  Serial.print("0x");
  for (int i=0; i<length; i++) {
    if (data[i]<0x10) {Serial.print("0");}
    Serial.print(data[i],HEX);
    Serial.print(" ");
  }
}

void buildPacket(char _ssid[], int len, char _mac[], uint8_t _ch){
  if(len > 32) len = 32;
  _packetSize = 0;
  
  for(int i=0;i<sizeof(packetBegin);i++) _packet[_packetSize+i] = packetBegin[i];
  _packetSize += sizeof(packetBegin);
  
  _packet[_packetSize] = 0x00;
  _packet[_packetSize+1] = len;
  _packetSize += 2;

  for(int i=0;i<len;i++) _packet[_packetSize+i] = _ssid[i];
  _packetSize += len;

  for(int i=0;i<6;i++) _packet[10+i] = _packet[16+i] = _mac[i];
  
  for(int i=0;i<sizeof(packetEnd);i++) _packet[_packetSize+i] = packetEnd[i];
  _packetSize += sizeof(packetEnd);

  _packet[_packetSize] = _ch;

  if(spamWPA){
    for(int i=0;i<sizeof(wpa2tag);i++) _packet[_packetSize+i] = wpa2tag[i];
    _packetSize += sizeof(wpa2tag);
  }
}

bool _send(uint8_t buf[], int len){
  delay(1);
  if(wifi_send_pkt_freedom(buf, len, 0) == -1){
    /* 
    //for debugging:
    Serial.print(_packetSize);
    Serial.print(" : ");
    PrintHex8(_packet, _packetSize);
    Serial.println("");
    */
    return false;
  }else return true;
}

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  Serial.println("");
  Serial.println("");

  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(1); 

  randomSeed(os_random());
  
  Serial.print("Initializing SD card...");

  if(SD.begin(chipSelect)) {
    Serial.println("initialization done.");
    ssdiFile = SD.open(fileName);
    if(ssdiFile) {
      ssidNum = 0;
      while(ssdiFile.available() != 0 && ssidNum < maxSize) {
        ssid = ssdiFile.readStringUntil('\n');
        if(ssid == "") break;
        ssid.replace("\r","");
        ssid.toCharArray(ssids[ssidNum],maxLen);
        ssidNum++;
        Serial.println(ssid);
      } 
      ssdiFile.close();
    }else Serial.println("error opening ssid.txt");
  }else{
    Serial.println("initialization failed!");
    Serial.println("using default names");
  }

  for(int i=0;i<ssidNum;i++){
    ssid = ssids[i];
    ssidLengths[i] = ssid.length();
    macs[i][0] = 0x14;
    macs[i][1] = 0x56;
    macs[i][2] = 0x8E;
    for(int h=3;h<6;h++) macs[i][h] = random(256);
    if(randomChannel) channels[i] = random(1,12);
    else channels[i] = channel;
  }
  Serial.println("starting...");
}

void loop(){

  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= 1000){
    previousMillis = currentMillis;
    for(int i=0;i<ssidNum;i++){
      buildPacket(ssids[i], ssidLengths[i], macs[i], channels[i]);
      wifi_set_channel(channels[i]);
      if(_send(_packet, _packetSize)){
        Serial.print(".");
        delay(1);
      }
      else Serial.print(" ");
    }
    Serial.println("");
  }

}
