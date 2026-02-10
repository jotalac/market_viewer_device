#include "BaseScreen.h"
#include "AiTextScreen.h"
#include "StockScreen.h"
#include "CryptoScreen.h"
#include "ClockScreen.h"

BaseScreen* createScreenFromType(const String& type, JsonObject& data) {
    int position = data["position"];

    if (type == "AI_TEXT") {
        return new AiTextScreen(
            position,
            data["fetchIntervalHours"],
            data["prompt"]
        );
    }
    
    if (type == "CLOCK") {
        ClockType clockType = ClockType::ANALOG_CLOCK;
        String clockTypeStr = data["clockType"];
        if (clockTypeStr == "DIGITAL") {
            clockType = ClockType::DIGITAL_CLOCK;
        }
        
        return new ClockScreen(
            position,
            data["timezone"],
            data["timezoneCode"],
            data["timeFormat"] == "TWENTY_FOUR_HOUR",
            clockType
        );
    }
    
    if (type == "CRYPTO") {
        return new CryptoScreen(
            position,
            data["assetName"],
            data["currency"],
            data["fetchInterval"],
            data["timeFrame"]
        );
    }
    
    if (type == "STOCK") {
        return new StockScreen(
            position,
            data["refreshInterval"],
            data["symbol"],
            data["timeFrame"]
        );
    }

    Serial.println("Unknown screen type: " + type);
    return nullptr;
}