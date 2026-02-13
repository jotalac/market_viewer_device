#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <string>
#include "ui.h"

std::string format_market_price(double price);
std::string currency_symbol_convertor(const String &currency);
std::string build_price_label(double price, String currency);
std::string build_price_change_label(double priceChange);

const lv_font_t* select_correct_font_size(std::string priceLabel, int labelWidth);

#endif