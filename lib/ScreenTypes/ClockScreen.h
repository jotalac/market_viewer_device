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

public:
    ClockScreen(int pos, String timezone, String timezoneCode, bool user24Hour, ClockType clockType) 
    : BaseScreen(pos), timezone(timezone), timezoneCode(timezoneCode), use24Hour(user24Hour), clockType(clockType) {
        type = ScreenType::CLOCK;
    }

    void parseData(JsonObject& data) override;

    void render() override;

    bool needsUpdate() override {return false;}

    String getDisplayName() override {
        return "Clock (" + timezone + ")";
    }

    // Clock-specific getters
    String getTimezone() const { return timezone; }
    bool is24Hour() const { return use24Hour; }
};

#endif