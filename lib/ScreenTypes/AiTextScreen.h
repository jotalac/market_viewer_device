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
        type = ScreenType::AI_TEXT;
        displayText = "";
        
        // set to infinity because we dont wanto to fetch automatically for the first time
    }

    void parseData(JsonObject& data) override;

    void render() override;
    void renderNoData();
    void renderData();

    bool needsUpdate() override;

    String getDisplayName() override {
        return "AI text (" + String(position) + ")";
    }

};

#endif