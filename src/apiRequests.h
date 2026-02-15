#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "./classes/GPSData.h"

extern const String device_id;

void uploadDetectedPing(GPSData& data) {
    if(WiFi.status() != WL_CONNECTED){
        Serial.println("Wifi Not Connected!");
        return;
    }

    const String api_url = "https://erbriwan-api.onrender.com/device/recordPing";

    WiFiClientSecure client;
    client.setInsecure();
    client.setTimeout(15);  // 15 second timeout
    client.setHandshakeTimeout(15);  // SSL handshake timeout

    HTTPClient http;
    http.setTimeout(20000);  // 20 second HTTP timeout
    
    if(!http.begin(client, api_url)){
        Serial.println("HTTP begin failed!");
        return;
    }
    
    http.addHeader("Content-Type", "application/json");

    String jsonStr = "";
    JsonDocument doc;

    doc["longitude"] = data.lon;
    doc["latitude"] = data.lat;
    doc["altitude"] = data.alt;
    doc["speed_kmph"] = data.spd;
    doc["device_id"] = data.device_id;
    doc["receiver_device_id"] = device_id;
    doc["ping_count"] = data.ping_count;
    doc["is_click"] = data.isClick;
    doc["is_cancel"] = data.isCancellation;

    serializeJson(doc, jsonStr);
    
    Serial.println("Sending POST request...");
    int respcode = http.POST(jsonStr);

    if(respcode > 0){
        Serial.print("RESPONSE CODE : ");
        Serial.println(respcode);
        if(respcode == 200 || respcode == 201){
            String response = http.getString();
            Serial.println("Response: " + response);
        }
    } else {
        Serial.print("HTTP POST failed, error: ");
        Serial.println(http.errorToString(respcode));
    }
    
    http.end();
    client.stop();

    return;
}