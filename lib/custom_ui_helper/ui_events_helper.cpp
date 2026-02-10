#include "ui_events_helper.h"
#include "ui.h"
#include <Arduino.h>
#include "WifiConfig.h"
#include "HardwareDriver.h"
#include "AudioManager.h"
#include <Preferences.h>

static Preferences preferences;

void changeWifiScreenNotConnected() {
    //change the connection status
    lv_label_set_text(ui_connectionLabel, "NOT CONNECTED");
    lv_obj_set_style_bg_color(ui_wifiConnectionRoundIndicator, lv_color_hex(_ui_theme_color_redDark[0]), LV_PART_MAIN);
    //display the current wifif ssid
    lv_obj_add_flag(ui_connectedWifiContainer, LV_OBJ_FLAG_HIDDEN);
}

void changeWifiScreenConnected() {
    //change the connection status
    lv_label_set_text(ui_connectionLabel, "CONNECTED");
    lv_obj_set_style_bg_color(ui_wifiConnectionRoundIndicator, lv_color_hex(0x1ec20c), LV_PART_MAIN);
    //display the current wifif ssid
    lv_label_set_text(ui_connectedWifiSSIDLabel, get_connected_ssid());
    lv_obj_clear_flag(ui_connectedWifiContainer, LV_OBJ_FLAG_HIDDEN);
}

void displayWifiConnectCredentials(bool shouldDisplay) {
    if (shouldDisplay) {
        lv_obj_clear_flag(ui_overlayWifiContainer, LV_OBJ_FLAG_HIDDEN);
        lv_label_set_text(ui_overlayWifiPwdLabelValue, wifiPassword.c_str());
        lv_label_set_text(ui_overlayWifiSSIDLabelValue, wifiSSID.c_str());
        
        lv_refr_now(NULL);
        delay(100);
    } else {
        lv_obj_add_flag(ui_overlayWifiContainer, LV_OBJ_FLAG_HIDDEN);
    }

}

void changeHomeScreenWifiIcon(bool isConnected) {
    if (isConnected) {
        lv_obj_set_style_bg_color(ui_wifiButton, lv_color_hex(_ui_theme_color_yellowDark[0]), LV_PART_MAIN);
    } else {
        lv_obj_set_style_bg_color(ui_wifiButton, lv_color_hex(_ui_theme_color_redDark[0]), LV_PART_MAIN);
    }
}

//hardware screens
void saveHardwaveNumberToPreferences(String name, int value) {
    preferences.begin("hardware_config", false);
    preferences.putInt(name.c_str(), value);
    preferences.end();
}
void saveHardwaveBoolToPreferences(String name, bool value) {
    preferences.begin("hardware_config", false);
    preferences.putBool(name.c_str(), value);
    preferences.end();
}