#ifndef CRYPTO_SCREEN_H
#define CRYPTO_SCREEN_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "BaseScreen.h"
#include <vector>
#include "graphDrawer.h"

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

    //graph data
    std::vector<double> graphData;
    lv_obj_t* graphPanel;
    GraphDrawContext graphContext;


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

        graphPanel = nullptr;
    }

    void parseData(JsonObject& data) override;
    void render() override;
    bool needsUpdate() override;

    void initGraph(lv_obj_t* panelObj);
    void renderGraph();


    String getDisplayName() override {
        return "Crypto: " + assetName;
    }

    // Add method to clear graph data when needed
    void clearGraphData() {
        graphData.clear();
        graphData.shrink_to_fit();
        if (graphPanel) {
            lv_obj_invalidate(graphPanel);
        }
    }

};


#endif