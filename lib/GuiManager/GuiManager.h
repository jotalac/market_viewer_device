#pragma once
#include <lvgl.h>

void init_lvgl_interface();
void update_gui(); 

// Call this from Main to rotate the screen
void handle_screen_rotation();
void set_software_rotation(int rotation_code);

void load_screen_by_index(int index, bool goingFromSettings);
void go_next_screen();
void go_prev_screen();

void go_back_from_market_data_setting();

//handle screen update
void updateScreen();
void updateScreenForce();