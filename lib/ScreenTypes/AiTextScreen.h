#ifndef AI_TEXT_SCREEN_H
#define AI_TEXT_SCREEN_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "BaseScreen.h"

class AiTextScreen : public BaseScreen {
private:
    int refreshIntervalHours;
    String prompt;
    String displayText;

public:
    AiTextScreen(int pos, int refreshIntervalHours, String prompt) 
    : BaseScreen(pos), refreshIntervalHours(refreshIntervalHours), prompt(prompt) {
        type = ScreenType::CLOCK;
        displayText = "";
    }

    void parseData(JsonObject& data) override {
        displayText = data["displayText"] | "";
    }

    void render() override {
        Serial.println("Rendering Clock Screen");
        // TODO: Implement clock rendering with LVGL
    }

    void update() override {
        // Update time display
    }

    String getDisplayName() override {
        return "Ai text (" + String(position) + ")";
    }

};

#endif