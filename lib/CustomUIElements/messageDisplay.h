#ifndef MESSAGE_DISPLAY_H
#define MESSAGE_DISPLAY_H

#include <Arduino.h>
#include <vector>

enum MessageSeverity {
    INFO,
    ERROR,
    SUCCESS
};

const char* severityToString(MessageSeverity severity);
int severityToColor(MessageSeverity severity);

void display_message(String message, MessageSeverity severity);
void destroy_message();

void show_error_message(String message);
void show_success_message(String message);
void show_info_message(String message);


#endif