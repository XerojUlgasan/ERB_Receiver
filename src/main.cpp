#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>
#include <esp_task_wdt.h>

#include "./classes/GPSData.h"
#include "webServerHandler.h"
#include "classes/myGps/MyGps.h"
#include "classes/receiverProfile/ReceiverProfile.h"
#include "classes/myLora/MyLora.h"

const bool deviceIsSender = false;

Preferences pref;

ResponderType res;
MyGps gps;
MyLora lora(5, 14, 26);

void setup() {
  esp_task_wdt_init(15, true);

  WiFi.softAP("ERBriwan_Receiver", "PitelBurat123");
  Serial.begin(115200);

  if(initializeWebServer(deviceIsSender, pref)){
    Serial.println("Setup Done!");
  }else{
    Serial.println("Setup Failed!");
    ESP.restart();
  }

  gps.begin();
  lora.begin();
  lora.startReceive();
}
int count = 0;
void loop() { 
  // Serial.println("MY LOCATIONNNNNNNNN" + gps.locationToJsonString());

  // lora.sendPacket((String)count++);
  // lora.sendPacket(gps.locationToJsonString());
  // if (WiFi.status() == WL_CONNECTED) {
  //   Serial.print("WiFi is connected to: ");
  //   Serial.println(WiFi.SSID());
  //   Serial.println(WiFi.localIP());
  // }

  if(lora.packetReceived){
    lora.packetReceived = false;

    Serial.println("\nLoRa received a message...");

    Serial.println("LAT : " + (String)lora.gpsdata.lat);
    Serial.println("LON : " + (String)lora.gpsdata.lon);
    Serial.println("ALT : " + (String)lora.gpsdata.alt);
    Serial.println("SAT : " + (String)lora.gpsdata.sat);
    Serial.println("SPD : " + (String)lora.gpsdata.spd);
    Serial.println("ISV : " + (String)lora.gpsdata.isValid);

    lora.printDetails();
    lora.startReceive();
  }

}

