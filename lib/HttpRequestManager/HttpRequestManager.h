#ifndef HTTP_REQUEST_MANAGER_H
#define HTTP_REQUEST_MANAGER_H

#include <Arduino.h>
#include <vector>
#include <ArduinoJson.h>

String get_base_url();
bool fetch_data(String url, JsonDocument& doc);

bool fetch_screens(JsonDocument& doc);
String create_fetch_screens_url();

bool fetch_screen_data(int screenPosition, JsonDocument& doc);
String create_fetch_screen_data_url(int screenPosition);


#endif