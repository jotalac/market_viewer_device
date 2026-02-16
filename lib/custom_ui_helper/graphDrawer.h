#ifndef GRAPH_DRAWER_H
#define GRAPH_DRAWER_H

#include <Arduino.h>
#include <vector>
#include "ui.h"

// we need this because LVGL's callback only accepts one "user_data" pointer.
struct GraphDrawContext {
    std::vector<double>* data; // Pointer to the vector in your screen class
    lv_color_t color;          // The color (Green/Red)
    lv_color_t bgColor;        // Background (usually Black)
};

void draw_graph_on_canvas(lv_obj_t* panel, const std::vector<double>& graphData, lv_color_t color);
// void setup_graph_panel(lv_obj_t* panel, GraphDrawContext* context);


#endif