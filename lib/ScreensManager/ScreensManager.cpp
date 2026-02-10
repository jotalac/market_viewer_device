#include "ScreensManager.h"
#include "WifiConfig.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "screen_types/BaseScreen.h"

static std::vector<BaseScreen*> screens;
static Preferences preferences;
static HTTPClient http;
static int currentScreenIndex = 0;

void init_screens_manager() {
    Serial.println("Initializing Screens Manager...");
    currentScreenIndex = 0;
    screens.clear();
}

void clear_all_screens() {
    // Free memory for all screens
    for (auto screen : screens) {
        delete screen;
    }
    screens.clear();
    currentScreenIndex = 0;
    
    Serial.println("All screens cleared");
}



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
    
    String url = String(backendUrl) + "hardware/" + String(deviceHash) + "/screen";
    
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
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);
    
    if (error) {
        Serial.println("JSON parse error: " + String(error.c_str()));
        return false;
    }
    
    // Clear existing screens
    clear_all_screens();
    
    // Parse screens array
    JsonArray screensArray = doc.as<JsonArray>();
    
    for (JsonObject screenObj : screensArray) {
        String type = screenObj["screenType"];
        JsonObject data = screenObj;
        
        // Add position to data object for factory
        data["position"] = screenObj["position"];
        
        // Create screen using factory
        BaseScreen* screen = createScreenFromType(type, data);
        
        if (screen != nullptr) {
            screens.push_back(screen);
            Serial.println("Added screen: " + screen->getDisplayName());
        } else {
            Serial.println("Failed to create screen of type: " + type);
        }
    }
    
    Serial.println("Fetched " + String(screens.size()) + " screens");
    
    // Reset to first screen
    currentScreenIndex = 0;
    
    return true;
}