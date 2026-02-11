#ifndef SCREENS_MANAGER_H
#define SCREENS_MANAGER_H

#include <Arduino.h>
#include <vector>
#include "BaseScreen.h"

struct ScreenInfo {
    int position;
    String name;

    ScreenInfo(int pos, String n) : position(pos), name(n) {}
};

// Initialize the manager
// void init_screens_manager();

// Fetch screens from backend
bool get_screens_from_backend();

// Get all screens
std::vector<ScreenInfo> get_all_screens_info();

void sortScreens();

BaseScreen* get_screen_ptr(int index);

// Get screen by position
// ScreenInfo* get_screen_by_id(const String& position);

// Get screen count
int get_screen_count();

// Save screens to flash (cache)
// void save_screens_to_flash();

// Load screens from flash (cache)
// bool load_screens_from_flash();

// Clear all screens
void clear_all_screens();

#endif