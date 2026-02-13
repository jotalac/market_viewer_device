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
    int refreshIntervalMinutes;
    String timeFrame;

    //data
    double ath;
    double athChange;
    std::vector<double> graphData;
    double price;
    double priceChange;

public:
    CryptoScreen(int pos, String assetName, String currency, int refreshInterval, String timeFrame, bool displayGraph, bool simpleDisplay, GraphType graphType) 
    : BaseScreen(pos),
    assetName(assetName),
    currency(currency),
    refreshIntervalMinutes(refreshInterval),
    timeFrame(timeFrame),
    simpleDisplay(simpleDisplay),
    displayGraph(displayGraph),
    graphType(graphType)
    {
        type = ScreenType::CRYPTO;

        ath = 0.0f;
        athChange = 0.0f;
        graphData = {};
        price = 0.0f;
        priceChange = 0.0f;
    }

    void parseData(JsonObject& data) override;
    void render() override;
    bool needsUpdate() override;


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