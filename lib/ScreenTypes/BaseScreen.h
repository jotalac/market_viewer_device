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

public:
    BaseScreen(int pos) : position(pos) {}
    virtual ~BaseScreen() {}

    // Pure virtual methods - each screen type must implement
    virtual void parseData(JsonObject& data) = 0;
    virtual void render() = 0;
    virtual void update() = 0;
    virtual String getDisplayName() = 0;

    // Common getters
    int getPosition() const { return position; }
    ScreenType getType() const { return type; }
};

BaseScreen* createScreenFromType(const String& type, JsonObject& data);


#endif