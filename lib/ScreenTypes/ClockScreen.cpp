#include "ClockScreen.h"

void ClockScreen::parseData(JsonObject& data) {
    timezone = data["timezone"] | "Europe/London";
    timezoneCode = data["timezoneCode"] | "GMT0BST,M3.5.0/1,M10.5.0";
    clockType = data["clockType"] == "ANALOG" ? ClockType::ANALOG_CLOCK : ClockType::DIGITAL_CLOCK;
    use24Hour = data["timeFormat"] == "TWENTY_FOUR_HOUR";
}



void ClockScreen::render() {
    Serial.println("rendering");
}