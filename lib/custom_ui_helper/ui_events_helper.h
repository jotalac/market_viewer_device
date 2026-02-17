#ifndef UI_EVENTS_HELPER_H
#define UI_EVENTS_HELPER_H

#include <Arduino.h>

//wifi screen
void changeWifiScreenNotConnected();
void changeWifiScreenConnected();
void displayWifiConnectCredentials(bool shouldDisplay);

// home screen
void changeHomeScreenWifiIcon(bool isConnected);

//hardware screens
void saveHardwaveNumberToPreferences(String name, int value);
void saveHardwaveBoolToPreferences(String name, bool value);

//screens screen dispaly
void updateScreensScreenOnDataFetch(bool successfull);

// market data settings screen
void updateMarketDataScreenOnLoad();

// market data screen update data
void updateSimpleDisplay(bool isSimpleDisplay);
void updateDispalyGraph(bool displayGraph);
void updateCandleGraph(bool isCandleGraph);

#endif