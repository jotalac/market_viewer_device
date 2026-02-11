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
    int refreshInterval;  // minutes
    String symbol;
    String timeFrame;
    unsigned long lastFetchTime;

    //fetched data
    String currency;
    std::vector<double> graphData;
    bool isMarketOpen;
    String stockName;
    double currentPrice;
    double priceChange;

public:
    StockScreen(int pos, int refreshInterval, String symbol, String timeFrame) 
    : BaseScreen(pos), refreshInterval(refreshInterval), graphType(graphType), symbol(symbol), timeFrame(timeFrame) {
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

    void parseData(JsonObject& data) override {
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

    void render() override;

    void update() override {
        // Fetch latest price from API
        lastFetchTime = millis();
    }

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