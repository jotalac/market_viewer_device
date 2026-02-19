#include "ui_events_helper.h"
#include "ui.h"
#include <Arduino.h>
#include "WifiConfig.h"
#include "HardwareDriver.h"
#include "AudioManager.h"
#include "ScreensManager.h"
#include <Preferences.h>
#include "CryptoScreen.h"

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
    
    //display the backend url and device hash
    lv_label_set_text(ui_backendUrlLabel, get_backend_url());
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


// screens screen
void updateScreensScreenOnDataFetch(bool successfull) {
    if (!successfull) {
        lv_obj_add_flag(ui_screensListPanel, LV_OBJ_FLAG_HIDDEN);
        lv_label_set_text_fmt(ui_screenNotFetchedLabel, "Fetch error...");
        lv_obj_clear_flag(ui_screenNotFetchedLabel, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_clear_flag(ui_screensListPanel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_screenNotFetchedLabel, LV_OBJ_FLAG_HIDDEN);

        // Clear existing children in the list panel (except template if you have one)
        lv_obj_clean(ui_screensListPanel);
        
        std::vector<ScreenInfo> screens = get_all_screens_info();
        
        for (int i = 0; i < screens.size(); i++) {
            // Create a label for each screen
            lv_obj_t* label = lv_label_create(ui_screensListPanel);
            
            String text = String(screens[i].position + 1) + ". " + screens[i].name;
            lv_label_set_text(label, text.c_str());
            
            // Style the label
            lv_obj_set_width(label, 300);
            lv_obj_set_height(label, 50);    /// 1
            lv_obj_set_align(label, LV_ALIGN_CENTER);
            lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL);
            lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
            lv_obj_set_style_text_font(label, &ui_font_mono30, LV_PART_MAIN);
        }
    }
}

void updateMarketDataSettingsScreenOnLoad(BaseScreen* activeScreen) {    
    bool displayGraph = true;
    bool simpleDisplay = false;
    GraphType graphType = GraphType::LINE;

    ScreenType screenType = activeScreen->getType();

    //set the states
    if(screenType == ScreenType::CRYPTO) {
        CryptoScreen* crypto = static_cast<CryptoScreen*>(activeScreen);

        displayGraph = crypto->shouldDisplayGraph();
        simpleDisplay = crypto->isSimpleDisplay();
        graphType = crypto->getGraphType();
    }

    Serial.println("Display graph: " + displayGraph);
    Serial.println("Simple display: " + simpleDisplay);
    Serial.println("Candle graph: " + (graphType == GraphType::CANDLE));

    //update the ui
    if (simpleDisplay) {
        lv_obj_add_state(ui_simpleDisplaySwitch, LV_STATE_CHECKED);
    } else {
        lv_obj_clear_state(ui_simpleDisplaySwitch, LV_STATE_CHECKED);
    } 

    if (displayGraph) {
        lv_obj_add_state(ui_displayGraphSwitch, LV_STATE_CHECKED);
        
        //enable char switch
        lv_obj_clear_state(ui_candleChartSwitch, LV_STATE_DISABLED);
    } else {
        lv_obj_clear_state(ui_displayGraphSwitch, LV_STATE_CHECKED);
        
        //disable editing candle graph
        lv_obj_add_state(ui_candleChartSwitch, LV_STATE_DISABLED);
    } 

    if (graphType == GraphType::CANDLE) {
        lv_obj_add_state(ui_candleChartSwitch, LV_STATE_CHECKED);
    } else {
        lv_obj_clear_state(ui_candleChartSwitch, LV_STATE_CHECKED);
    } 

}

void updateSimpleDisplay(bool isSimpleDisplay, BaseScreen* activeScreen) {
    if (activeScreen->getType() == ScreenType::CRYPTO) {
        CryptoScreen* crypto = static_cast<CryptoScreen*>(activeScreen);

        crypto->setSimpleDisplay(isSimpleDisplay);
    }
}
void updateDispalyGraph(bool displayGraph, BaseScreen* activeScreen) {
    if (activeScreen->getType() == ScreenType::CRYPTO) {
        CryptoScreen* crypto = static_cast<CryptoScreen*>(activeScreen);

        crypto->setDisplayGraph(displayGraph);
    }

    if(displayGraph) {
        lv_obj_clear_state(ui_candleChartSwitch, LV_STATE_DISABLED);    
    } else {
        lv_obj_add_state(ui_candleChartSwitch, LV_STATE_DISABLED);
    }

}
void updateCandleGraph(bool isCandleGraph, BaseScreen* activeScreen) {
    if (activeScreen->getType() == ScreenType::CRYPTO) {
        CryptoScreen* crypto = static_cast<CryptoScreen*>(activeScreen);

        crypto->setGraphType(isCandleGraph);
    }
}