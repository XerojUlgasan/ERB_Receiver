#include <ESPAsyncWebServer.h>
#include <Preferences.h>

#include "./helpers/utils.h"

AsyncWebServer server(80);

bool initializeWebServer(const bool &deviceIsSender, Preferences &pref){

    try
    {
        server.on( // TYPE
            "/type", 
            HTTP_GET, 
            [deviceIsSender, &pref](AsyncWebServerRequest *request) {
            String json = "{\"isSender\": " +(String)deviceIsSender + "}";

            request->send(
            200,
            "application/json",
            json);
        });

        server.on(
        "/detectNetworks", // Returns all network 
        HTTP_GET,
        [](AsyncWebServerRequest *request){
            std::vector<String> ssid;
            std::vector<bool> isSecure;

            detectNetworks(ssid, isSecure);

            String ssidStr = vectorToJsonStringArr(ssid);
            String isSecureStr = vectorToJsonBoolArr(isSecure);

            Serial.println(ssidStr);
            Serial.println(isSecureStr);
            
            String json = "{";
            json += "\"networks\": " + ssidStr + ",";
            json += "\"isSecure\": " + isSecureStr;
            json += "}";

            Serial.println("Sending response...");
            request->send(200, "application/json", json);
            Serial.println("Response sent");
        });

        server.on(
        "/setNetwork",
        HTTP_POST,
        [](AsyncWebServerRequest *request, JsonVariant &json){
            String ssid = json["ssid"].as<String>();
            String pass = json["password"].as<String>();

            WiFi.mode(WIFI_AP_STA);
            WiFi.begin(ssid, pass);
            
            request->send(200);
        }
        );

        server.on(
        "/wifiStatus",
        HTTP_GET,
        [](AsyncWebServerRequest *request){
            String jsonString;
            JsonDocument doc;
            doc["ssid"] = (WiFi.status() == WL_CONNECTED) ? WiFi.SSID() : "Not Connected";
            doc["isConnected"] = (WiFi.status() == WL_CONNECTED) ? true : false;
            doc["ip"] = (WiFi.status() == WL_CONNECTED) ? WiFi.localIP().toString() : "192.168.4.1";

            serializeJson(doc, jsonString);

            request->send(
            200,
            "application/json",
            jsonString
            );
        }
        );

        return true;
    }
    catch(const std::exception& e)
    {
        Serial.println(e.what());
        return false;
    }
    
    return true;
}