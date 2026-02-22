#include "BaseScreen.h"
#include "AiTextScreen.h"
#include "StockScreen.h"
#include "CryptoScreen.h"
#include "ClockScreen.h"
#include "HttpRequestManager.h"
#include "messageDisplay.h"
#include "Arduino.h"

void BaseScreen::update() {
    display_message("Fetching new data...", MessageSeverity::INFO);

    String jsonPayload = fetch_screen_data(position);

    //check if we got error fetching the data
    if (jsonPayload.isEmpty()) {
        Serial.println("Failed to fetch screens - empty response");
        handleUpdateError("Error fetching data...");
        return;
    }

    // Parse JSON here where the document stays in scope
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, jsonPayload);
    
    if (error) {
        Serial.println("JSON parse error: " + String(error.c_str()));
        handleUpdateError("Failed to parse response...");
        return;
    }
    
    JsonObject screenData = doc.as<JsonObject>();
    
    if (screenData.size() == 0) {
        Serial.println("No screen data found");
        handleUpdateError("Error - empty response...");
        return;
    }

    destroy_message();
    show_success_message("Screen data updated");

    parseData(screenData);
    render();

    lastFetchTime = millis();
}

void BaseScreen::handleUpdateError(String message) {
    destroy_message();
    show_error_message(message);

    // fetch failed try again in a minute
    lastFetchTime = millis();
}

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
        GraphType graphType = data["graphType"] == "CANDLE" ? GraphType::CANDLE : GraphType::LINE;
        return new CryptoScreen(
            position,
            data["assetName"],
            data["currency"],
            data["fetchInterval"] | 10,
            data["timeFrame"],
            data["displayGraph"],
            data["simpleDisplay"],
            graphType
        );
    }
    
    if (type == "STOCK") {
        GraphType graphType = data["graphType"] == "CANDLE" ? GraphType::CANDLE : GraphType::LINE;
        return new StockScreen(
            position,
            data["symbol"],
            data["fetchInterval"] | 10,
            data["timeFrame"],
            data["displayGraph"],
            data["simpleDisplay"],
            graphType
        );
    }

    Serial.println("Unknown screen type: " + type);
    return nullptr;
}


// BaseScreen* createTestScreenFromType(const ScreenType& type, int position) {
//     if (type == ScreenType::CRYPTO) {
//     return new CryptoScreen(
//         position,
//         "bitcoin",
//         "usd",
//         10,
//         "24h",
//         true,
//         false,
//         GraphType::LINE
//     );
//     }
    
//     if (type == ScreenType::STOCK) {
//         return new StockScreen(
//             position,
//             10,
//             "AAPL",
//             "1day"
//     );}

//     return nullptr;
// }
