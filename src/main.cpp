#include <Arduino.h>
#include "HardwareDriver.h"
#include "AudioManager.h"
#include "GuiManager.h"
#include "WifiConfig.h"
#include "startupEvents.h"
#include "ScreensManager.h"

void setup() {
  Serial.begin(115200);
  
  //Initialize Hardware (Screen, Touch, I2C)
  init_hardware();

  //Initialize audio 
  init_audio_manager();
  
  //try to connect to saved wifi
  init_wifi_config();
  
  //Initialize LVGL (Buffers, UI)
  init_lvgl_interface();

  // Load saved hardware settings (brightness, volume, rotation)
  loadHardwareSettingsFromPreferences();
  
  // add_test_screens();
  Serial.println("System Ready!");
}

void loop() {
    // 1. Update GUI
    update_gui();
  
    updateScreen();

    delay(5);
}