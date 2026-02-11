#ifndef CRYPTO_SCREEN_H
#define CRYPTO_SCREEN_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "BaseScreen.h"
#include <vector>

// Crypto Screen
class CryptoScreen : public BaseScreen {
private:
    //setting options
    bool displayGraph;
    bool simpleDisplay;
    GraphType graphType;

    String assetName;
    String currency;
    int refreshInterval;
    String timeFrame;
    unsigned long lastFetchTime;

    //data
    double ath;
    double athChange;
    std::vector<double> graphData;
    double price;
    double priceChange;

public:
    CryptoScreen(int pos, String assetName, String currency, int refreshInterval, String timeFrame) 
    : BaseScreen(pos), assetName(assetName), currency(currency), refreshInterval(refreshInterval), timeFrame(timeFrame) {
        type = ScreenType::CRYPTO;
        displayGraph = true;
        simpleDisplay = false;
        graphType = GraphType::LINE;

        ath = 0.0f;
        athChange = 0.0f;
        graphData = {};
        price = 0.0f;
        priceChange = 0.0f;
    }

    void parseData(JsonObject& data) override {
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

    void render() override;

    void update() override {
        // Fetch latest crypto price from API
        lastFetchTime = millis();
    }

    String getDisplayName() override {
        return "Crypto: " + assetName;
    }

    // // Crypto-specific methods
    // String getCoinId() const { return coinId; }
    // String getCoinSymbol() const { return coinSymbol; }
    // float getCurrentPrice() const { return currentPrice; }
    // float getChange24h() const { return changePercent24h; }
    // void setPrice(float price, float change) {
    //     currentPrice = price;
    //     changePercent24h = change;
    // }
};


#endif