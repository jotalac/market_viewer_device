#include "ui.h" // Import the SquareLine generated UI
#include "CryptoScreen.h"

void CryptoScreen::render() {
    lv_label_set_text(ui_cryptoPriceLabel, String(price).c_str());
}