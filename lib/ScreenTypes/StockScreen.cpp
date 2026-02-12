#include "StockScreen.h"
#include "ui.h" // Import the SquareLine generated UI

void StockScreen::render() {
    lv_label_set_text(ui_stockPriceLabel, String(currentPrice).c_str());
}

void StockScreen::parseData(JsonObject& data) {
    currency = data["currency"] | "USD";
    currentPrice = data["price"] | 0.0f;
    isMarketOpen = data["isMarketOpen"] | false;
    stockName = data["name"] | "";
    priceChange = data["priceChange"] | 0.0f;
    
    // Parse graph data array manually
    graphData.clear();
    JsonArray graphArray = data["graphData"].as<JsonArray>();
    for (JsonVariant value : graphArray) {
        graphData.push_back(value.as<double>());
    }
}

bool StockScreen::needsUpdate() {
    unsigned long refreshIntervalMillis = refreshIntervalMinutes * 60 * 1000;

    // Serial.println(String(refreshIntervalMinutes) + " refresh interval minutes");

    // Serial.println(String(refreshIntervalMillis) + " refresh interval millis");
    // Serial.println(String(lastFetchTime) + " last fetch time");
    // Serial.println(String(millis()) + " current millis");
    // Serial.println("");

    return (millis() - lastFetchTime >= refreshIntervalMillis);
}