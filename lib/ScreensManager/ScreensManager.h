#ifndef SCREENS_MANAGER_H
#define SCREENS_MANAGER_H

#include <Arduino.h>
#include <vector>

struct ScreenInfo {
    String position;
    String type;
    String data;  // JSON or other data for the screen
    bool active;
};

// Initialize the manager
// void init_screens_manager();

// Fetch screens from backend
bool fetch_screens_from_backend();

// Get all screens
std::vector<ScreenInfo>& get_all_screens();

// Get screen by position
ScreenInfo* get_screen_by_id(const String& position);

// Get screen count
int get_screen_count();

// Save screens to flash (cache)
// void save_screens_to_flash();

// Load screens from flash (cache)
// bool load_screens_from_flash();

// Clear all screens
void clear_all_screens();

#endif