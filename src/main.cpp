#include <Arduino.h>
#include "HardwareDriver.h"
#include "AudioManager.h"
#include "GuiManager.h"
#include "WifiConfig.h"

void setup() {
  Serial.begin(115200);
  
  //Initialize Hardware (Screen, Touch, I2C)
  init_hardware();

  //Initialize audio 
  init_audio_manager();
  
  //Initialize LVGL (Buffers, UI)
  init_lvgl_interface();

  //try to connect to saved wifi
  init_wifi_config();
  
  Serial.println("System Ready!");
}

void loop() {
    // 1. Update GUI
    update_gui();
  

    delay(5);
}