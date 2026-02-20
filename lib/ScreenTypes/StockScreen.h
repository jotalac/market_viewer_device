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
    bool isMarketOpen;
    String stockName;
    double price;
    double priceChange;

    std::vector<double> graphData;

public:
    StockScreen(int pos, String symbol, int refreshInterval, String timeFrame, bool displayGraph, bool simpleDisplay, GraphType graphType) 
    : BaseScreen(pos),
    symbol(symbol),
    refreshIntervalMinutes(refreshInterval),
    timeFrame(timeFrame),
    simpleDisplay(simpleDisplay),
    displayGraph(displayGraph),
    graphType(graphType)
    {
        type = ScreenType::STOCK;

        currency = "???";
        price = 0.0f;
        priceChange = 0.0f;
        graphData = {};
        isMarketOpen = false;
        stockName = "???";
    }

    void parseData(JsonObject& data) override;
    void render() override;
    bool needsUpdate() override;

    void renderNormal();
    void renderSimple();
    void renderGraph();

    String getDisplayName() override {return "Stock: " + symbol;}

    bool isSimpleDisplay() {return simpleDisplay;}
    bool shouldDisplayGraph() {return displayGraph;}
    GraphType getGraphType() {return graphType;}

    void setSimpleDisplay(bool simpleDisplay) {this->simpleDisplay = simpleDisplay;}
    void setDisplayGraph(bool displayGraph) {this->displayGraph = displayGraph;}
    void setGraphType(bool isCandle) {this->graphType = isCandle ? GraphType::CANDLE : GraphType::LINE;}
};


#endif