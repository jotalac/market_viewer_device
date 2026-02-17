#include "utils.h"
#include <algorithm> 
#include <cstdio>
#include <format>


std::string format_market_price(double price) {
    if (price == 0) {
        return "???";
    }
    // Use snprintf for float formatting with thousands separator
    char buffer[32];
    
    // Format based on price size
    if (price >= 1000) {
        // Large prices: format with commas
        int whole = (int)price;
        std::string result = "";
        int count = 0;
        
        while (whole > 0) {
            if (count > 0 && count % 3 == 0) {
                result += ' ';
            }
            result += (char)((whole % 10) + '0');
            whole /= 10;
            count++;
        }
        std::reverse(result.begin(), result.end());
        return result;
        
    } else if (price >= 1) {
        // prices 1-999: show 2 decimals
        snprintf(buffer, sizeof(buffer), "%.2f", price);
        
    } else if (price >= 0.01) {
        // prices 0.01-0.99: show 4 decimals
        snprintf(buffer, sizeof(buffer), "%.4f", price);        
    } else {
        // Very small prices: show 8 decimals
        snprintf(buffer, sizeof(buffer), "%.8f", price);
    }
    return std::string(buffer);
}

std::string currency_symbol_convertor(const String &currency) {
    String currencyUpper = currency;
    currencyUpper.toUpperCase();

    if (currencyUpper == "USD") return "$";  // US Dollar
    if (currencyUpper == "EUR") return "€";  // Euro
    if (currencyUpper == "GBP") return "£";  // British Pound
    if (currencyUpper == "JPY") return "¥";  // Japanese Yen
    if (currencyUpper == "CNY") return "¥";  // Chinese Yuan (Renminbi)
    
    return std::string(currency.c_str()); // Return original code if no symbol found
    // return ""; // Return original code if no symbol found
}

std::string build_price_label(double price, String currency) {
    std::string formattedPrice = format_market_price(price);
    formattedPrice += currency_symbol_convertor(currency);
    return formattedPrice;
}

std::string build_price_change_label(double priceChange) {
    return std::format("{:+.2f}%", priceChange);
}

const lv_font_t* availableFonts[] = {
    &ui_font_monoBold100,
    &ui_font_monoBold80,
    &ui_font_monoBold60,
    &ui_font_monoBold50,
};

const lv_font_t* select_correct_font_size(std::string priceLabel, int labelWidth) {
    int fontCount = 4;
    // Iterate through all available fonts
    for (int i = 0; i < fontCount; i++) {
        const lv_font_t* font = availableFonts[i];
        
        // 1. Measure the text width with this specific font
        lv_point_t size_res;
        lv_txt_get_size(
            &size_res, 
            priceLabel.c_str(), 
            font, 
            0,  // Letter spacing (usually 0)
            0,  // Line spacing (irrelevant for 1 line)
            LV_COORD_MAX, // Max width (we want the full theoretical width)
            LV_TEXT_FLAG_NONE
        );

        // 2. Check if it fits
        if (size_res.x <= labelWidth) {
            return font; // Found the biggest one that fits!
        }
    }

    // fallback: If nothing fits, return the smallest font 
    return availableFonts[3];
}