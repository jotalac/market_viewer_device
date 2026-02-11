SET(SOURCES ui_homeScreen.c
    ui_brightnessScreen.c
    ui_volumeScreen.c
    ui_wifiScreen.c
    ui_screensScreen.c
    ui_cryptoScreen.c
    ui_stockScreen.c
    ui_theme_manager.cpp
    ui_themes.cpp
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_events.cpp
    ui_img_1314322640.c
    ui_img_1826884842.c
    ui_img_367695115.c
    ui_img_871200326.c
    ui_img_555948897.c
    ui_img_1272477160.c
    ui_img_1702299517.c
    ui_img_536642073.c
    ui_img_9953116.c
    ui_img_788343386.c
    ui_img_936315526.c
    fonts/ui_font_Mono20.c
    fonts/ui_font_mono30.c
    fonts/ui_font_Mono40.c
    fonts/ui_font_MonoBold100.c
    fonts/ui_font_MonoBold60.c)

add_library(ui ${SOURCES})
