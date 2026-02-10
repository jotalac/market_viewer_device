#include "ScreensManager.h"
#include "WifiConfig.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Preferences.h>

static std::vector<ScreenInfo> screens;
static Preferences preferences;
static HTTPClient http;


bool fetch_screens_from_backend() {
    if (!is_wifi_connected()) {
        Serial.println("Cannot fetch screens - WiFi not connected");
        return false;
    }
    
    const char* backendUrl = get_backend_url();
    const char* deviceHash = get_device_hash();
    
    if (strlen(backendUrl) == 0 || strlen(deviceHash) == 0) {
        Serial.println("Backend URL or device hash not configured");
        return false;
    }
    
    String url = String(backendUrl) + "hardware/" + String(deviceHash) + "/screens";
    
    Serial.println("Fetching screens from: " + url);
    
    http.begin(url);
    http.setTimeout(10000);  // 10 second timeout
    
    int httpCode = http.GET();
    
    if (httpCode != HTTP_CODE_OK) {
        Serial.println("HTTP error: " + String(httpCode));
        http.end();
        return false;
    }
    
    String payload = http.getString();
    http.end();
    
    Serial.println("Received: " + payload);
    
    // Parse JSON response
    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, payload);
    
    if (error) {
        Serial.println("JSON parse error: " + String(error.c_str()));
        return false;
    }
    
    // Clear existing screens
    screens.clear();
    
    // Parse screens array
    JsonArray screensArray = doc["screens"].as<JsonArray>();
    
    for (JsonObject screenObj : screensArray) {
        ScreenInfo screen;
        screen.position = screenObj["position"].as<String>();
        screen.type = screenObj["type"].as<String>();
        screen.data = screenObj["data"].as<String>();
        screen.active = screenObj["active"] | true;
        
        screens.push_back(screen);
        // Serial.println("Added screen: " + screen.name);
    }
    
    Serial.println("Fetched " + String(screens.size()) + " screens");
    
    // Cache to flash
    // save_screens_to_flash();
    
    return true;
}