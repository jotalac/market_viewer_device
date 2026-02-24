#include "ClockScreen.h"
#include "ui.h"
#include <time.h>

// void ClockScreen::parseData(JsonObject& data) {
//     timezone = data["timezone"] | "Europe/London";
//     timezoneCode = data["timezoneCode"] | "GMT0BST,M3.5.0/1,M10.5.0";
//     clockType = data["clockType"] == "ANALOG" ? ClockType::ANALOG_CLOCK : ClockType::DIGITAL_CLOCK;
//     use24Hour = data["timeFormat"] == "TWENTY_FOUR_HOUR";

//     configTzTime(timezoneCode.c_str(), "pool.ntp.org", "time.nist.gov");
// }

bool ClockScreen::needsUpdate() {    
    // update the data here, we dont want to fetch from our standart backend api
    if (millis() - lastFetchTime >= refreshIntervalMillis) {
        configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    }
    return false;
}


void ClockScreen::updateClockNeedles() {
    static uint32_t last_tick = 0;
    
    // Only update the math once a second to save CPU
    if (millis() - last_tick > 1000) {
        last_tick = millis();

        // 1. Get the raw time (This ALWAYS works, even before Wi-Fi connects)
        time_t now;
        time(&now);

        // 2. Convert to local time safely based on the timezone code
        struct tm timeinfo;
        localtime_r(&now, &timeinfo); 

        hour = timeinfo.tm_hour;
        minute = timeinfo.tm_min;
        second = timeinfo.tm_sec;

        char timeText[16]; 
        snprintf(timeText, sizeof(timeText), "%02d:%02d", hour, minute);

        // calculate angles for needles
        int32_t second_angle = second * 60; 
        int32_t minute_angle = minute * 60; 
        int32_t hour_angle = ((hour % 12) * 300) + (minute * 5); 
        
        // update all needles
        lv_img_set_angle(ui_clockNeedleMinute, minute_angle);        
        lv_img_set_angle(ui_clockNeedleSecond, second_angle);

        //update digial clock
        lv_label_set_text(ui_timeLabel, timeText);
    }
}

void ClockScreen::render() {
    //set the timezone for the specific clock
    setenv("TZ", timezoneCode.c_str(), 1);
    tzset();

    //set timezone text
    lv_label_set_text(ui_timezoneLabel, timezone.c_str());
}