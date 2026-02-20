#include "StockScreen.h"
#include "ui.h"
#include "messageDisplay.h"
#include "HttpRequestManager.h"
#include "utils.h"
#include "string"
#include "graphDrawer.h"
#include "colors.h"

void StockScreen::render() {
    //update current price and asset name (same for simple display and normal)
    std::string formattedPrice = build_price_label(price, currency).c_str();

    String tempName = String(symbol.c_str());
    tempName.toUpperCase();
    std::string assetPlusCurrency = (tempName + " " + currency).c_str();

    //select right font size based on price label size
    const lv_font_t* correctFont = select_correct_font_size(formattedPrice, 450);
    lv_obj_set_style_text_font(ui_cryptoPriceLabel, correctFont, LV_PART_MAIN);

    //set values
    lv_label_set_text(ui_stockPriceLabel, formattedPrice.c_str());
    lv_label_set_text(ui_stockTimeFrameLabel, timeFrame.c_str());
    lv_label_set_text(ui_stockSymbolCurrencyLabel, assetPlusCurrency.c_str());
    
    //update colors
    if (priceChange >= 0) {
        lv_obj_set_style_bg_img_src(ui_stockScreen, &ui_img_green_background_png, LV_PART_MAIN);
    } else {
        lv_obj_set_style_bg_img_src(ui_stockScreen, &ui_img_red_background_png, LV_PART_MAIN);
    }

    //handle simple display and normal display
    if (simpleDisplay) {
        renderSimple();
    } else { 
        renderNormal();
    }

    //clear graph data
    lv_obj_clean(get_screen_panel_from_type(ScreenType::STOCK));

    //display graph data
    if (displayGraph) {
        renderGraph();
    }
}

void StockScreen::renderNormal() {
    //show all widgets
    lv_obj_clear_flag(ui_stockPriceChangeLabel, LV_OBJ_FLAG_HIDDEN);    
    lv_obj_clear_flag(ui_stockNameLabel, LV_OBJ_FLAG_HIDDEN);    
    lv_obj_clear_flag(ui_stockMarketOpenCont, LV_OBJ_FLAG_HIDDEN);   

    //update widgets values
    std::string formattedPriceChange = build_price_change_label(priceChange).c_str();
    lv_label_set_text(ui_stockPriceChangeLabel, formattedPriceChange.c_str());    

    //handle market dat open
    if (isMarketOpen) {
        lv_label_set_text(ui_stockMarketOpenLabel, "OPEN");
        lv_obj_set_style_bg_color(ui_stockMarketOpenIcon, lv_color_hex(greenColor), LV_PART_MAIN);
    } else {
        lv_label_set_text(ui_stockMarketOpenLabel, "CLOSED");
        lv_obj_set_style_bg_color(ui_stockMarketOpenIcon, lv_color_hex(redColor), LV_PART_MAIN);
    }

    lv_label_set_text(ui_stockNameLabel, stockName.c_str());

    //chagne colors based on price change
    if (priceChange >= 0) {
        lv_obj_set_style_bg_color(ui_stockPriceChangeLabel, lv_color_hex(greenColor), LV_PART_MAIN);
    } else {
        lv_obj_set_style_bg_color(ui_cryptoPriceChangeLabel, lv_color_hex(redColor), LV_PART_MAIN);
        lv_obj_set_style_bg_color(ui_cryptoAthArc, lv_color_hex(redColorKnob), LV_PART_KNOB);
    }
 
}
void StockScreen::renderSimple() {
    //hide widgets for simple display
    lv_obj_add_flag(ui_stockPriceChangeLabel, LV_OBJ_FLAG_HIDDEN);    
    lv_obj_add_flag(ui_stockNameLabel, LV_OBJ_FLAG_HIDDEN);    
    lv_obj_add_flag(ui_stockMarketOpenCont, LV_OBJ_FLAG_HIDDEN);     
}



void StockScreen::parseData(JsonObject& data) {
    currency = data["currency"] | "USD";
    price = data["price"] | 0.0f;
    isMarketOpen = data["isMarketOpen"] | false;
    stockName = data["name"] | "";
    priceChange = data["priceChange"] | 0.0f;
    
    // Parse graph data array manually
    graphData.clear();
    graphData.shrink_to_fit(); // Release memory

    // Parse new graph data
    JsonArray graphArray = data["graphData"].as<JsonArray>();
    if (graphArray.size() > 0) {
        // Reserve memory to prevent reallocation
        graphData.reserve(graphArray.size());
        
        for (JsonVariant value : graphArray) {
            graphData.push_back(value.as<double>());
        }        
    }
}

bool StockScreen::needsUpdate() {
    unsigned long refreshIntervalMillis = refreshIntervalMinutes * 60 * 1000;

    return (millis() - lastFetchTime >= refreshIntervalMillis);
}

void StockScreen::renderGraph() {
    if (graphData.empty()) return;

    // 1. Update the color based on logic
    lv_color_t graphColor = priceChange >= 0 ? lv_color_hex(greenColorGraph) : lv_color_hex(redColorGraph);
    draw_graph_on_canvas(ScreenType::STOCK, graphData, graphColor, graphType == GraphType::CANDLE);
}