#include "StockScreen.h"
#include "ui.h" // Import the SquareLine generated UI

void StockScreen::render() {
    lv_label_set_text(ui_stockPriceLabel, String(currentPrice).c_str());
}