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
    double price;
    double priceChange;

    std::vector<double> graphData;

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

    void renderNormal();
    void renderSimple();
    void renderGraph();

    String getDisplayName() override {return "Crypto: " + assetName;}
    
    bool isSimpleDisplay() {return simpleDisplay;}
    bool shouldDisplayGraph() {return displayGraph;}
    GraphType getGraphType() {return graphType;}

    void setSimpleDisplay(bool simpleDisplay) {this->simpleDisplay = simpleDisplay;}
    void setDisplayGraph(bool displayGraph) {this->displayGraph = displayGraph;}
    void setGraphType(bool isCandle) {this->graphType = isCandle ? GraphType::CANDLE : GraphType::LINE;}
};


#endif