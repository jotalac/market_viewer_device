#ifndef STOCK_SCREEN_H
#define STOCK_SCREEN_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "BaseScreen.h"
#include <vector>


// Stock Screen
class StockScreen : public BaseScreen {
private:
    //setting options
    bool displayGraph;
    bool simpleDisplay;
    GraphType graphType;

    //seted data
    int refreshIntervalMinutes;  // minutes
    String symbol;
    String timeFrame;

    //fetched data
    String currency;
    std::vector<double> graphData;
    bool isMarketOpen;
    String stockName;
    double currentPrice;
    double priceChange;

public:
    StockScreen(int pos, int refreshInterval, String symbol, String timeFrame) 
    : BaseScreen(pos), refreshIntervalMinutes(refreshInterval), graphType(graphType), symbol(symbol), timeFrame(timeFrame) {
        type = ScreenType::STOCK;
        displayGraph = true;
        simpleDisplay = false;
        graphType = GraphType::LINE;

        currency = "???";
        currentPrice = 0.0f;
        priceChange = 0.0f;
        graphData = {};
        isMarketOpen = false;
        stockName = "???";
    }

    void parseData(JsonObject& data) override;
    void render() override;
    bool needsUpdate() override;

    String getDisplayName() override {
        return "Stock: " + symbol;
    }

    // Stock-specific methods
    String getSymbol() const { return symbol; }
    // String getCompanyName() const { return companyName; }
    float getCurrentPrice() const { return currentPrice; }
    // float getChangePercent() const { return changePercent; }
    // void setPrice(float price, float change) { 
        // currentPrice = price; 
        // changePercent = change; 
    // }
};


#endif