#include "graphDrawer.h"
#include <algorithm>
#include "colors.h"

static lv_point_t pointBuffer[400]; 

void draw_candle_graph(lv_obj_t* panel, size_t pointCount) {
    Serial.println("Rendering candle graph");
    // --- 2. SAFE CANDLE GRAPH ---
    lv_color_t colorGreen = lv_color_hex(redColorGraph);
    lv_color_t colorRed = lv_color_hex(greenColorGraph);

    for (size_t i = 1; i < pointCount; i++) {
        int32_t prev_x = pointBuffer[i-1].x;
        int32_t prev_y = pointBuffer[i-1].y;
        int32_t current_x = pointBuffer[i].x;
        int32_t current_y = pointBuffer[i].y;

        // Width with a safe 2px gap
        int32_t rect_w = current_x - prev_x - 2; 
        if (rect_w < 2) rect_w = 2; // Prevent zero-width crash

        int32_t rect_y = std::min(prev_y, current_y);
        int32_t rect_h = std::abs(prev_y - current_y);
        if (rect_h < 3) rect_h = 3; // Ensure visible dash for flat prices

        lv_color_t candleColor = (current_y <= prev_y) ? colorGreen : colorRed;

        lv_obj_t* candle = lv_obj_create(panel);
        lv_obj_remove_style_all(candle); 
        
        lv_obj_set_size(candle, rect_w, rect_h);
        lv_obj_set_pos(candle, prev_x, rect_y);
        
        lv_obj_set_style_bg_color(candle, candleColor, 0);
        lv_obj_set_style_bg_opa(candle, LV_OPA_COVER, 0);
        lv_obj_set_style_radius(candle, 5, LV_PART_MAIN);
    }

}

void draw_line_graph(lv_obj_t* panel, size_t pointCount, lv_color_t color) {
    // --- Render Line Graph ---
    lv_obj_t* line = lv_line_create(panel);
    lv_line_set_points(line, pointBuffer, pointCount);
    lv_obj_set_style_line_width(line, 4, LV_PART_MAIN);
    lv_obj_set_style_line_color(line, color, LV_PART_MAIN);
    lv_obj_set_style_line_rounded(line, true, LV_PART_MAIN);
}


void draw_graph_on_canvas(ScreenType screenType, const std::vector<double>& rawData, lv_color_t color, bool isCandleGraph) {
    // 1. Safety Checks
    if (rawData.size() < 2) return;

    lv_obj_t* panel = get_screen_panel_from_type(screenType);
    if (panel == nullptr) return;

    // 3. Get Dimensions
    int32_t w = lv_obj_get_width(panel);
    int32_t h = lv_obj_get_height(panel);
    
    // Safety check for tiny/hidden panels
    if (w < 5 || h < 5) return;

    size_t totalDataPoints = rawData.size();
    size_t step = 1;
    
    int minPixelsPerPoint = isCandleGraph ? 20 : 1; 
    size_t maxPoints = w / minPixelsPerPoint;
    if (totalDataPoints > maxPoints) step = totalDataPoints / maxPoints;

    double minVal = rawData[0];
    double maxVal = rawData[0];
    for (size_t i = 0; i < totalDataPoints; i += step) {
        if (rawData[i] < minVal) minVal = rawData[i];
        if (rawData[i] > maxVal) maxVal = rawData[i];
    }
    
    double range = maxVal - minVal;
    if (range < 0.000001) range = 1.0;
    
    double padding = range * 0.1;
    minVal -= padding;
    maxVal += padding;
    range = maxVal - minVal;

    size_t pointCount = 0;
    for (size_t i = 0; i < totalDataPoints; i += step) {
        if (pointCount >= 400) break; 

        double normalized = (rawData[i] - minVal) / range;
        if (normalized < 0.0) normalized = 0.0;
        if (normalized > 1.0) normalized = 1.0;

        pointBuffer[pointCount].x = (lv_coord_t)((i * w) / (totalDataPoints - 1));
        pointBuffer[pointCount].y = (lv_coord_t)(h - (normalized * h));
        pointCount++;
    }

    if (pointCount > 0) pointBuffer[0].x = 0;
    if (pointCount > 1) pointBuffer[pointCount-1].x = w - 1;

    
    if (!isCandleGraph) {
        draw_line_graph(panel, pointCount, color);
    } else {
        draw_candle_graph(panel, pointCount);
    }
}

lv_obj_t* get_screen_panel_from_type(ScreenType screenType) {
    if (screenType == ScreenType::CRYPTO) {
        return ui_cryptoGraphPanel;
    } else if (screenType == ScreenType::STOCK) {
        return ui_stockGraphPanel; // TODO return crypto graph data
    }

    return nullptr;
}
