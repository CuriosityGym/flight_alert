#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <EEPROM.h>


WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  replyPacekt[] = "Hi there! Got the message :-)";  // a reply string to send back
int motorPin = 2;


void setup()
{
  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin,LOW);
  Serial.begin(115200);
  Serial.println();
  
   WiFiManager wifiManager;
  // wifiManager.resetSettings();
     wifiManager.autoConnect("GoAir");
     Serial.println("connected...yeey :)");
     Serial.println( WiFi.SSID().c_str());
     Serial.println(WiFi.psk().c_str());
    //String ssid1 = WiFi.SSID().c_str();
    wifiManager.setConfigPortalTimeout(180);

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}


void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
    
    // send back a reply, to the IP address and port we got the packet from
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyPacekt);
    Udp.endPacket();
    int recievedData = atoi(incomingPacket);
    if(recievedData == 15 )
      {
         digitalWrite(motorPin, HIGH);
         delay(500);
         digitalWrite(motorPin, LOW);
         delay(500);
         digitalWrite(motorPin, HIGH);
         delay(500);
         digitalWrite(motorPin, LOW);
         delay(500);
      } 
  }
  
        
    
}
