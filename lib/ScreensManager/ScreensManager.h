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

void add_test_screens();

// Fetch screens from backend
bool get_screens_from_backend();

// Get all screens
std::vector<ScreenInfo> get_all_screens_info();

void sortScreens();

BaseScreen* get_screen_ptr(int index);

int get_screen_count();

// Clear all screens
void clear_all_screens();

#endif