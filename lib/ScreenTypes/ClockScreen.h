#ifndef CLOCK_SCREEN_H
#define CLOCK_SCREEN_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "BaseScreen.h"

// Clock Screen
enum class ClockType {
    ANALOG_CLOCK,
    DIGITAL_CLOCK
};

class ClockScreen : public BaseScreen {
private:
    String timezone;
    String timezoneCode;
    bool use24Hour;
    ClockType clockType;

    int hour, minute, second;

    // 24h
    unsigned long refreshIntervalMillis = 24 * 60 * 60 * 1000;

public:
    ClockScreen(int pos, String timezone, String timezoneCode, bool user24Hour, ClockType clockType) 
    : BaseScreen(pos), timezone(timezone), timezoneCode(timezoneCode), use24Hour(user24Hour), clockType(clockType) {
        type = ScreenType::CLOCK;
        hour = 0;
        minute = 0;
        second = 0;

        // get the current time
        configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    }

    void parseData(JsonObject& data) override {}

    void render() override;

    bool needsUpdate() override;

    String getDisplayName() override {
        return "Clock (" + timezone + ")";
    }

    void updateClockNeedles();


    // Clock-specific getters
    String getTimezone() const { return timezone; }
    bool is24Hour() const { return use24Hour; }
};

#endif