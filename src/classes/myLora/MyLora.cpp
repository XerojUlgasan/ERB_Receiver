#include <Arduino.h>
#include <LoRa.h>

#include "MyLora.h"

void onReceive(int packetSize);
void onReceiveStruct(int packetSize);

// Define static member variables
String MyLora::receivedMessage = "";
bool MyLora::packetReceived = false;
GPSData MyLora::gpsdata = {};

MyLora::MyLora(int nss, int rst, int dio)
{
    _nss = nss;
    _rst = rst;
    _dio = dio;
}

MyLora::~MyLora(){}

void MyLora::begin() {
    LoRa.setPins(_nss, _rst, _dio);

    if(!LoRa.begin(433E6)){
        Serial.println("LoRa init failed!");
        ESP.restart();
    }

    LoRa.setSpreadingFactor(12);
    LoRa.setSignalBandwidth(41.7E3);
    LoRa.setCodingRate4(8);
    LoRa.setTxPower(20);
    LoRa.enableCrc();
    LoRa.setSyncWord(0x1f);
    LoRa.setPreambleLength(12);

    LoRa.onReceive(onReceiveStruct);

    Serial.println("LoRa has started...");
}

void MyLora::sendPacket(String message){

    Serial.println("Sending packet...");

    LoRa.beginPacket();
    LoRa.print("Hello from ESP32");
    LoRa.endPacket();

    Serial.println("LoRa sent : " + message);

    return;
}

void MyLora::sendPacketStruct(GPSData &gpsData){
    Serial.println("Sending Struct....");

    LoRa.beginPacket();
    LoRa.write((uint8_t*)&gpsData, sizeof(gpsData));
    LoRa.endPacket();

    Serial.println("Lora Send Struct");
}

void MyLora::startReceive(){
    LoRa.receive();
    Serial.println("LoRa is listening...");

    return;
}

void MyLora::stopReceive(){
    LoRa.idle();
    Serial.println("LoRa is idling...");

    return;
}

void MyLora::printDetails() {
    Serial.println("RSSI : " + (String)LoRa.packetRssi());
    Serial.println("SNR  : " + (String)LoRa.packetSnr());
}

void onReceive(int packetSize){
    if(packetSize == 0) return;
    
    MyLora::receivedMessage = "";

    while (LoRa.available()) {
        MyLora::receivedMessage += (char)LoRa.read();
    }

    MyLora::packetReceived = true;
    return;
}

void onReceiveStruct(int packetSize){    
    Serial.print("Received packet size: ");
    Serial.print(packetSize);
    Serial.print(" bytes, Expected: ");
    Serial.print(sizeof(GPSData));
    Serial.println(" bytes");
    
    try
    {
        if(packetSize == sizeof(GPSData)) {
            LoRa.readBytes((uint8_t*)&MyLora::gpsdata, sizeof(MyLora::gpsdata));
            MyLora::packetReceived = true;
            Serial.println("Struct received successfully!");
        } else {
            Serial.println("Packet size mismatch! Ignoring packet.");
            while(LoRa.available()) {
                LoRa.read();
            }
        }
    }
    catch(const std::exception& e)
    {
        Serial.print("Error receiving struct: ");
        Serial.println(e.what());
    }
}



