#ifndef GRAPH_DRAWER_H
#define GRAPH_DRAWER_H

#include <Arduino.h>
#include <vector>
#include "ui.h"
#include "BaseScreen.h"

void draw_graph_on_canvas(ScreenType screenType, const std::vector<double>& graphData, lv_color_t color, bool isCandleGraph);

lv_obj_t* get_screen_panel_from_type(ScreenType screenType);

#endif