#include "messageDisplay.h"
#include "ui.h"


lv_obj_t * ui_messageContainer = NULL;
lv_obj_t * ui_messageLabel = NULL;
lv_obj_t * ui_messageInfoLabel = NULL;

void display_message(String message, MessageSeverity severity) {
    ui_messageContainer = lv_obj_create(lv_scr_act());
    lv_obj_set_width(ui_messageContainer, 480);
    lv_obj_set_height(ui_messageContainer, 200);
    lv_obj_set_x(ui_messageContainer, 0);
    lv_obj_set_y(ui_messageContainer, -135);
    lv_obj_set_align(ui_messageContainer, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_messageContainer, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_messageContainer, lv_color_hex(0x748501), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_messageContainer, 200, LV_PART_MAIN);
    lv_obj_set_style_border_color(ui_messageContainer, lv_color_hex(0xD0D0D0), LV_PART_MAIN);
    lv_obj_set_style_border_opa(ui_messageContainer, 255, LV_PART_MAIN);

    ui_messageLabel = lv_label_create(ui_messageContainer);
    lv_obj_set_width(ui_messageLabel, 374);
    lv_obj_set_height(ui_messageLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_messageLabel, 0);
    lv_obj_set_y(ui_messageLabel, 30);
    lv_obj_set_align(ui_messageLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_messageLabel, message.c_str());
    lv_obj_set_style_text_color(ui_messageLabel, lv_color_hex(severityToColor(severity)), LV_PART_MAIN);
    lv_obj_set_style_text_opa(ui_messageLabel, 255, LV_PART_MAIN);
    lv_obj_set_style_text_align(ui_messageLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_text_font(ui_messageLabel, &ui_font_mono30, LV_PART_MAIN);

    ui_messageInfoLabel = lv_label_create(ui_messageContainer);
    lv_obj_set_width(ui_messageInfoLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_messageInfoLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_messageInfoLabel, 0);
    lv_obj_set_y(ui_messageInfoLabel, -70);
    lv_obj_set_align(ui_messageInfoLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_messageInfoLabel, severityToString(severity));
    ui_object_set_themeable_style_property(ui_messageInfoLabel, LV_PART_MAIN, LV_STYLE_TEXT_COLOR,
                                           _ui_theme_color_yellowDark);
    ui_object_set_themeable_style_property(ui_messageInfoLabel, LV_PART_MAIN, LV_STYLE_TEXT_OPA,
                                           _ui_theme_alpha_yellowDark);
    lv_obj_set_style_text_font(ui_messageInfoLabel, &ui_font_Mono20, LV_PART_MAIN);

    // Force LVGL to render now
    lv_refr_now(NULL);
    delay(10);  // Small delay to ensure display updates
}

void show_error_message(String message) {
    display_message(message, MessageSeverity::ERROR);
    delay(500);
    destroy_message();
}

void show_info_message(String message) {
    display_message(message, MessageSeverity::INFO);
    delay(500);
    destroy_message();
}

void destroy_message() {
    if(ui_messageContainer) lv_obj_del(ui_messageContainer);

    // NULL screen variables
    ui_messageContainer = NULL;
    ui_messageLabel = NULL;
    ui_messageInfoLabel = NULL;
}



const char* severityToString(MessageSeverity severity) {
    switch (severity) {
        case MessageSeverity::INFO:    return "info";
        case MessageSeverity::ERROR:   return "error";
        default:                       return "unknown";
    }
}

int severityToColor(MessageSeverity severity) {
    switch (severity) {
        case MessageSeverity::INFO:    return 0xe8e8e8;
        case MessageSeverity::ERROR:   return 0xff6721;
        default:                       return 0xFFFFFF;
    }
}