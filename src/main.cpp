#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>
#include <esp_task_wdt.h>

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

void loop() { 

  delay(5000);
  Serial.println("MY LOCATIONNNNNNNNN" + gps.locationToJsonString());

  lora.sendPacket("LAMAOOOO");
  // if (WiFi.status() == WL_CONNECTED) {
  //   Serial.print("WiFi is connected to: ");
  //   Serial.println(WiFi.SSID());
  //   Serial.println(WiFi.localIP());
  // }

}
