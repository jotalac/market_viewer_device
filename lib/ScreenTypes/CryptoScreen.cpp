#include "ui.h" // Import the SquareLine generated UI
#include "CryptoScreen.h"
#include "messageDisplay.h"
#include "HttpRequestManager.h"
#include "utils.h"
#include "string"

static int redColor = 0xDF0A0A;
static int redColorKnob = 0xe35252;
static int greenColor = 0x399c09;
static int greenColorKnob = 0x7cd154;


void CryptoScreen::render() {
    std::string formattedPrice = build_price_label(price, currency).c_str();
    std::string formattedPriceChange = build_price_change_label(priceChange).c_str();
    double athBarValue = 100 + athChange;

    String tempName = String(assetName.c_str());
    tempName.toUpperCase();
    std::string assetPlusCurrency = (tempName + " " + currency).c_str();

    //select right font size based on price label size
    const lv_font_t* correctFont = select_correct_font_size(formattedPrice, 450);
    lv_obj_set_style_text_font(ui_cryptoPriceLabel, correctFont, LV_PART_MAIN);

    //set values
    lv_label_set_text(ui_cryptoPriceLabel, formattedPrice.c_str());
    lv_label_set_text(ui_cryptoPriceChangeLabel, formattedPriceChange.c_str());
    lv_label_set_text(ui_cryptoTimeFrameLabel, timeFrame.c_str());
    lv_label_set_text(ui_cryptoAssetCurrencyLabel, assetPlusCurrency.c_str());
    lv_arc_set_value(ui_cryptoAthArc, (int32_t)athBarValue);
    
    //update colors
    if (priceChange >= 0) {
        lv_obj_set_style_bg_color(ui_cryptoPriceChangeLabel, lv_color_hex(greenColor), LV_PART_MAIN);
        lv_obj_set_style_bg_img_src(ui_cryptoScreen, &ui_img_green_background_png, LV_PART_MAIN);
        lv_obj_set_style_bg_color(ui_cryptoAthArc, lv_color_hex(greenColorKnob), LV_PART_KNOB);
    } else {
        lv_obj_set_style_bg_color(ui_cryptoPriceChangeLabel, lv_color_hex(redColor), LV_PART_MAIN);
        lv_obj_set_style_bg_img_src(ui_cryptoScreen, &ui_img_red_background_png, LV_PART_MAIN);
        lv_obj_set_style_bg_color(ui_cryptoAthArc, lv_color_hex(redColorKnob), LV_PART_KNOB);
    }

    lv_obj_clean(graphPanel); 
    renderGraph();
}


void CryptoScreen::parseData(JsonObject& data) {
    price = data["price"] | 0.0f;
    priceChange = data["priceChange"] | 0.0f;
    ath = data["allTimeHigh"] | 0.0f;
    athChange = data["allTimeHighChange"] | 0.0f;
    
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

bool CryptoScreen::needsUpdate() {
    unsigned long refreshIntervalMillis = refreshIntervalMinutes * 60 * 1000;
    
    return (millis() - lastFetchTime >= refreshIntervalMillis);
}

void CryptoScreen::initGraph(lv_obj_t* panelObj) {
    this->graphPanel = panelObj;

    // 1. Configure the Context
    // We point it to OUR graphData vector. 
    // This is valid as long as 'CryptoScreen' stays alive.
    graphContext.data = &this->graphData; 
    graphContext.color = lv_color_hex(0x00FF00); // Default color (will update in render)
    graphContext.bgColor = lv_color_hex(0x000000);

    // 2. Call the reusable setup function
    // setup_graph_panel(graphPanel, &graphContext);
}

void CryptoScreen::renderGraph() {
    if (!graphPanel || graphData.empty()) return;


    // 1. Update the color based on logic
    bool isGrowing = (priceChange >= 0);
    graphContext.color = isGrowing ? lv_color_hex(0x399c09) : lv_color_hex(0xDF0A0A);

    // 2. Trigger Redraw
    // lv_obj_invalidate(graphPanel);
    // lv_refr_now(NULL);
    draw_graph_on_canvas(graphPanel, graphData, graphContext.color);
}