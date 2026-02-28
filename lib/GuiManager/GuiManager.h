#pragma once
#include <lvgl.h>
#include "BaseScreen.h"

void init_lvgl_interface();
void update_gui(); 

// Call this from Main to rotate the screen
void handle_screen_rotation();
void set_software_rotation(int rotation_code);

void load_screen_by_index(int index, bool goingFromSettings);
void go_next_screen();
void go_prev_screen();

void go_back_from_market_data_setting();
BaseScreen* get_active_screen();
void go_to_home_screen();

//handle screen update
void updateScreen();
void updateScreenForce();