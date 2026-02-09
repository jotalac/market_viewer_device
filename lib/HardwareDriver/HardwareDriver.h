#pragma once
#include <Arduino.h>
#include "Arduino_GFX_Library.h"
#include "TouchDrvCSTXXX.hpp"
#include <SensorQMI8658.hpp>
#include "ESP_I2S.h"
#include "es8311.h"

// Expose objects for advanced usage if needed
extern Arduino_CO5300 *gfx;
extern SensorQMI8658 qmi;
extern I2SClass i2s;
extern es8311_handle_t es_handle;

extern int currentBrightness;
extern int currentVolume;
extern bool shouldRotate;
extern bool isScreenOff;

// Core Functions
void init_hardware();
void init_audio();
bool get_touch(int16_t &x, int16_t &y);

void set_brightness(uint8_t val);
void set_brightness_percentage(uint8_t valPercentage);

void set_volume_percentage(uint8_t valPercentage);

// Helper to get accelerometer data
bool get_accel(float &x, float &y, float &z);