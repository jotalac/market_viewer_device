#include "ScreensManager.h"
#include "HttpRequestManager.h"
#include "WifiConfig.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "screen_types/BaseScreen.h"

static std::vector<BaseScreen*> screens;
static Preferences preferences;

static int currentScreenIndex = 0;

// void init_screens_manager() {
//     Serial.println("Initializing Screens Manager...");
//     currentScreenIndex = 0;
//     screens.clear();
// }

void clear_all_screens() {
    // Free memory for all screens
    for (auto screen : screens) {
        delete screen;
    }
    screens.clear();
    currentScreenIndex = 0;
    
    Serial.println("All screens cleared");
}

void sortScreens() {
    std::sort(screens.begin(), screens.end(), [](const BaseScreen* lhs, const BaseScreen* rhs) {
        return lhs->getPosition() < rhs->getPosition();
    });
}


bool get_screens_from_backend() {
    String jsonPayload = fetch_screens_from_backend();

    //check if we got error fetching the data
    if (jsonPayload.isEmpty()) {
        Serial.println("Failed to fetch screens - empty response");
        return false;
    }

    // Parse JSON here where the document stays in scope
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, jsonPayload);
    
    if (error) {
        Serial.println("JSON parse error: " + String(error.c_str()));
        return false;
    }
    
    JsonArray screensArray = doc.as<JsonArray>();
    
    if (screensArray.size() == 0) {
        Serial.println("No screens found in response");
        return false;
    }
    
    clear_all_screens();
    
    for (JsonObject screenObj : screensArray) {
        String type = screenObj["screenType"];
        JsonObject data = screenObj;
                
        // Create screen using factory
        BaseScreen* screen = createScreenFromType(type, data);
        
        if (screen != nullptr) {
            screens.push_back(screen);
            Serial.println("Added screen: " + screen->getDisplayName());
        } else {
            Serial.println("Failed to create screen of type: " + type);
        }
    }

    //sort screens
    sortScreens();
    
    Serial.println("Fetched " + String(screens.size()) + " screens");
    currentScreenIndex = 0;
    
    return true;
}

std::vector<ScreenInfo> get_all_screens_info() {
    std::vector<ScreenInfo> allScreenInfo = {};
    for (auto screen : screens) {
        allScreenInfo.push_back(ScreenInfo(screen->getPosition(), screen->getDisplayName()));
    }

    return allScreenInfo;
}