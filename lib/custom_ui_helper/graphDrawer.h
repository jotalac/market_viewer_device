#ifndef GRAPH_DRAWER_H
#define GRAPH_DRAWER_H

#include <Arduino.h>
#include <vector>
#include "ui.h"
#include "BaseScreen.h"

// we need this because LVGL's callback only accepts one "user_data" pointer.
struct GraphDrawContext {
    std::vector<double>* data; // Pointer to the vector in your screen class
    lv_color_t color;          // The color (Green/Red)
    lv_color_t bgColor;        // Background (usually Black)
};

void draw_graph_on_canvas(ScreenType screenType, const std::vector<double>& graphData, lv_color_t color);
lv_obj_t* get_screen_panel_from_type(ScreenType screenType);

#endif