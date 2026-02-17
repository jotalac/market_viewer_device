#include "graphDrawer.h"
#include <algorithm>

// Static buffer to hold points for the active graph.
// This prevents heap fragmentation from constantly allocating/freeing vectors.
// 400 points is enough for any screen width on ESP32.
static lv_point_t pointBuffer[400]; 

void draw_graph_on_canvas(ScreenType screenType, const std::vector<double>& rawData, lv_color_t color) {
    // 1. Safety Checks
    if (rawData.size() < 2) return;

    lv_obj_t* panel = get_screen_panel_from_type(screenType);
    if (panel == nullptr) return;

    // 3. Get Dimensions
    int32_t w = lv_obj_get_width(panel);
    int32_t h = lv_obj_get_height(panel);
    
    // Safety check for tiny/hidden panels
    if (w < 5 || h < 5) return;

    // 4. Downsampling Logic
    // Only process enough points to fill the screen width (1 point per pixel)
    size_t totalDataPoints = rawData.size();
    size_t step = 1;
    if (totalDataPoints > w) {
        step = totalDataPoints / w;
    }

    // 5. Calculate Min/Max (Range)
    double minVal = rawData[0];
    double maxVal = rawData[0];
    for (size_t i = 0; i < totalDataPoints; i += step) {
        if (rawData[i] < minVal) minVal = rawData[i];
        if (rawData[i] > maxVal) maxVal = rawData[i];
    }
    
    double range = maxVal - minVal;
    if (range < 0.000001) range = 1.0;
    
    // Add 10% vertical padding
    double padding = range * 0.1;
    minVal -= padding;
    maxVal += padding;
    range = maxVal - minVal;

    // 6. Fill the Point Buffer
    size_t pointCount = 0;
    
    for (size_t i = 0; i < totalDataPoints; i += step) {
        // Prevent buffer overflow
        if (pointCount >= 400) break; 

        // Normalize Y (0.0 to 1.0)
        double normalized = (rawData[i] - minVal) / range;
        
        // Clamp 
        if (normalized < 0.0) normalized = 0.0;
        if (normalized > 1.0) normalized = 1.0;

        // Map to Screen Coordinates
        // X: simple linear map
        lv_coord_t x = (lv_coord_t)((i * w) / (totalDataPoints - 1));
        
        // Y: Invert (0 is top)
        lv_coord_t y = (lv_coord_t)(h - (normalized * h));

        pointBuffer[pointCount].x = x;
        pointBuffer[pointCount].y = y;
        pointCount++;
    }
    // 7. Create the Line Object (The Widget)
    lv_obj_t* line = lv_line_create(panel);
    
    // Set the points (LVGL reads from our static buffer)
    lv_line_set_points(line, pointBuffer, pointCount);

    // 8. Style the Line
    lv_obj_set_style_line_width(line, 4, LV_PART_MAIN);
    lv_obj_set_style_line_color(line, color, LV_PART_MAIN);
    lv_obj_set_style_line_rounded(line, true, LV_PART_MAIN);
}

lv_obj_t* get_screen_panel_from_type(ScreenType screenType) {
    if (screenType == ScreenType::CRYPTO) {
        return ui_cryptoGraphPanel;
    } else if (screenType == ScreenType::STOCK) {
        return nullptr; // TODO return crypto graph data
    }

    return nullptr;
}