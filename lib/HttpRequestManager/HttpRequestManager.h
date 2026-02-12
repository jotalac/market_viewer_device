#ifndef HTTP_REQUEST_MANAGER_H
#define HTTP_REQUEST_MANAGER_H

#include <Arduino.h>
#include <vector>

String get_base_url();
String fetch_data(String url);

String fetch_screens();
String create_fetch_screens_url();

String fetch_screen_data(int screenPosition);
String create_fetch_screen_data_url(int screenPosition);


#endif