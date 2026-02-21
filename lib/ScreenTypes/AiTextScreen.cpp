#include "AiTextScreen.h"
#include "ui.h"

void AiTextScreen::parseData(JsonObject& data) {
    displayText = data["displayText"] | "";
}

bool AiTextScreen::needsUpdate() {
    unsigned long refreshIntervalMillis = refreshIntervalHours * 3600000UL;
    
    return (millis() - lastFetchTime >= refreshIntervalMillis);
}


void AiTextScreen::render() {
    if (displayText == "") {
        renderNoData();
    } else {
        renderData();
    }
}

void AiTextScreen::renderNoData() {
    //hide text data
    lv_obj_add_flag(ui_aiTextPanel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_state(ui_aiTextReadButton, LV_STATE_DISABLED);

    //show prompt
    lv_obj_clear_flag(ui_aiTextScreenPromptPanel, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui_aiPromptLabel, prompt.c_str());
}

void AiTextScreen::renderData() {
    //hide prompt data
    lv_obj_add_flag(ui_aiTextScreenPromptPanel, LV_OBJ_FLAG_HIDDEN);
    
    //show display data
    lv_obj_clear_flag(ui_aiTextPanel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_state(ui_aiTextReadButton, LV_STATE_DISABLED);

    lv_label_set_text(ui_aiTextLabel, displayText.c_str());
}