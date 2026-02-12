#include "ui.h" // Import the SquareLine generated UI
#include "CryptoScreen.h"
#include "messageDisplay.h"
#include "HttpRequestManager.h"

void CryptoScreen::render() {
    lv_label_set_text(ui_cryptoPriceLabel, String(price).c_str());
}

void CryptoScreen::parseData(JsonObject& data) {
    price = data["price"] | 0.0f;
    priceChange = data["priceChange"] | 0.0f;
    ath = data["allTimeHigh"] | 0.0f;
    athChange = data["allTimeHighChange"] | 0.0f;

    // Parse graph data array manually
    graphData.clear();
    JsonArray graphArray = data["graphData"].as<JsonArray>();
    for (JsonVariant value : graphArray) {
        graphData.push_back(value.as<double>());
    }

}

bool CryptoScreen::needsUpdate() {
    unsigned long refreshIntervalMillis = refreshIntervalMinutes * 60 * 1000;

    return (millis() - lastFetchTime >= refreshIntervalMillis);
}