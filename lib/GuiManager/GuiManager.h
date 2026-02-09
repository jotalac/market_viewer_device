#pragma once
#include <lvgl.h>

void init_lvgl_interface();
void update_gui(); 

// Call this from Main to rotate the screen
void handle_screen_rotation();
void set_software_rotation(int rotation_code);