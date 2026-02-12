#include "AiTextScreen.h"

void AiTextScreen::parseData(JsonObject& data) {
    displayText = data["displayText"] | "";
}

bool AiTextScreen::needsUpdate() {
    unsigned long refreshIntervalMillis = refreshIntervalHours * 3600000UL;
    
    if (millis() - lastFetchTime >= refreshIntervalMillis) {
        return true;
    }

    return false;
}


void AiTextScreen::render() {
    Serial.println("rendering");
}