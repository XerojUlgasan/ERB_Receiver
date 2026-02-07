#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>
#include <WiFi.h>

bool parseJsonToSenderProfile(String &jsonStr, String &fn, String &add, String &con_num, String &em_con_per, String &em_con_num){
  JsonDocument doc;

  DeserializationError err = deserializeJson(doc, jsonStr);

  if(err){
    Serial.print("Parse failed: ");
    Serial.println(err.c_str());
    return false;
  }

  try
  {
    fn = doc["fullname"].as<String>();
    add = doc["address"].as<String>();
    con_num = doc["contact_number"].as<String>();
    em_con_per = doc["emergency_contact_person"].as<String>();
    em_con_num = doc["emergency_contact_number"].as<String>();
  }
  catch(const std::exception& e)
  {
    Serial.println(e.what());
    return false;
  }

  return true;
}

void detectNetworks(std::vector<String> &ssid, std::vector<bool> &isSecure) {
    delay(100);

    int n = WiFi.scanNetworks(false, true); // false = not async, true = show hidden networks
    // Note: We use sync mode but with proper timeout handling in the web handler

    if(n == 0){
        Serial.println("No Network Detected");
    }else{
        Serial.println((String)n + " Network Detected");

        for(int i = 0; i < n; i++){
            ssid.push_back(WiFi.SSID(i));
            isSecure.push_back((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? false : true);

            Serial.println("\nNetwork  : " + WiFi.SSID(i));
            Serial.println("Security : " + String((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Open" : "Secured"));
        }
    }

    WiFi.scanDelete();

    return;
}

String appendPostBody(String &body, uint8_t *data, size_t len, size_t index, size_t total) {

    for (size_t i = 0; i < len; i++) {
        body += (char)data[i];
    }

    if (index + len == total) {
        return body;
    }
    return "";
}

String vectorToJsonStringArr(const std::vector<String>& vec) {
  String result;
  result.reserve(128);

  result += "[";

  for (size_t i = 0; i < vec.size(); i++) {
    result += "\"";
    result += vec[i];
    result += "\"";

    if (i < vec.size() - 1) result += ",";
  }

  result += "]";
  return result;
}

String vectorToJsonBoolArr(const std::vector<bool>& vec) {
  String result = "[";

  for (size_t i = 0; i < vec.size(); i++) {
    result += vec[i];
    if (i < vec.size() - 1) result += ", ";
  }

  result += "]";

  return result;
}