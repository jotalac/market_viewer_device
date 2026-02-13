#ifndef BASE_SCREEN_H
#define BASE_SCREEN_H

#include <Arduino.h>
#include <ArduinoJson.h>

// Screen type identifiers
enum class ScreenType {
    CLOCK,
    STOCK,
    CRYPTO,
    AI_TEXT
};

enum class GraphType {
    LINE,
    CANDLE
};

// Base Screen class
class BaseScreen {
protected:
    int position;
    ScreenType type;
    unsigned long lastFetchTime;

public:
    BaseScreen(int pos) : position(pos), lastFetchTime(0) {}
    virtual ~BaseScreen() {}

    // Pure virtual methods - each screen type must implement
    virtual void parseData(JsonObject& data) = 0;
    virtual void render() = 0;
    virtual String getDisplayName() = 0;
    virtual bool needsUpdate() = 0;

    void update();
    void handleUpdateError(String message);

    // Common getters
    int getPosition() const { return position; }
    ScreenType getType() const { return type; }
    void resetLastFetchTime() {lastFetchTime = millis()+10000000;} // needs to be fixed and doen proper
};

BaseScreen* createScreenFromType(const String& type, JsonObject& data);

// BaseScreen* createTestScreenFromType(const ScreenType& type, int position);


#endif