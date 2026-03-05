#pragma once
#include <Arduino.h>

extern String wifiSSID;
extern String wifiPassword;

struct WifiConfig {
    char backendUrl[256];
    char deviceHash[256];
    bool configured;
};

// initialize WiFi config (load saved settings)
void init_wifi_config();

// start WiFi Manager portal (blocking)
bool start_wifi_portal();

// check if WiFi is connected
bool is_wifi_connected();

//get connected wifi info
const char* get_connected_ssid();
String get_ip_address();

// get current configuration
WifiConfig get_device_config();

const char* get_backend_url();
const char* get_device_hash();

// reset configuration (clear saved settings)
void reset_wifi_config();