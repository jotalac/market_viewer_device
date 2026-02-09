#include "HardwareDriver.h"
#include <Wire.h>
#include <pin_config.h>

Arduino_DataBus *bus = new Arduino_ESP32QSPI(
  LCD_CS, LCD_SCLK, LCD_SDIO0, LCD_SDIO1, LCD_SDIO2, LCD_SDIO3);

Arduino_CO5300 *gfx = new Arduino_CO5300(
  bus, LCD_RESET, 0, LCD_WIDTH, LCD_HEIGHT, 6, 0, 0, 0);

TouchDrvCST92xx touch;
SensorQMI8658 qmi;

int currentBrightness = 50;
int currentVolume = 50;
bool isScreenOff = false;
bool shouldRotate = true;

//audio
I2SClass i2s;
es8311_handle_t es_handle = NULL;
#define SAMPLE_RATE 16000

void init_audio() {
    // 1. Enable Power Amplifier
    pinMode(PA, OUTPUT);
    digitalWrite(PA, HIGH);
    
    // 2. Initialize I2S
    i2s.setPins(BCLKPIN, WSPIN, DIPIN, DOPIN, MCLKPIN);
    if (!i2s.begin(I2S_MODE_STD, SAMPLE_RATE, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO, I2S_STD_SLOT_BOTH)) {
        Serial.println("I2S init failed!");
        return;
    }
    
    // 3. Initialize ES8311 codec
    es_handle = es8311_create(0, ES8311_ADDRRES_0);
    if (es_handle == NULL) {
        Serial.println("ES8311 create failed!");
        return;
    }
    
    const es8311_clock_config_t es_clk = {
        .mclk_inverted = false,
        .sclk_inverted = false,
        .mclk_from_mclk_pin = true,
        .mclk_frequency = SAMPLE_RATE * 256,
        .sample_frequency = SAMPLE_RATE
    };
    
    if (es8311_init(es_handle, &es_clk, ES8311_RESOLUTION_16, ES8311_RESOLUTION_16) != ESP_OK) {
        Serial.println("ES8311 init failed!");
        return;
    }
    
    es8311_microphone_config(es_handle, false);
    es8311_voice_volume_set(es_handle, currentVolume, NULL);
    
    Serial.println("Audio initialized successfully!");
}

void init_hardware() {
    //init I2C and Pins
    Wire.begin(IIC_SDA, IIC_SCL);
    pinMode(TP_RESET, OUTPUT);
    digitalWrite(TP_RESET, HIGH); delay(50);
    
    //init touch
    touch.setPins(TP_RESET, TP_INT);
    touch.begin(Wire, 0x5A, IIC_SDA, IIC_SCL);
    touch.setMaxCoordinates(466, 466);
    touch.setMirrorXY(true, true);

    //init sensor
    qmi.begin(Wire, QMI8658_L_SLAVE_ADDRESS, IIC_SDA, IIC_SCL);
    qmi.configAccelerometer(SensorQMI8658::ACC_RANGE_4G, SensorQMI8658::ACC_ODR_1000Hz, SensorQMI8658::LPF_MODE_0);
    qmi.enableAccelerometer();

    //init Display
    gfx->begin();
    set_brightness_percentage(currentBrightness);

    init_audio();
}

bool get_touch(int16_t &x, int16_t &y) {
    int16_t x_raw[1], y_raw[1];
    // getPoint(x, y, number_of_points)
    if (touch.getPoint(x_raw, y_raw, 1)) {
        x = x_raw[0];
        y = y_raw[0];
        return true;
    }
    return false;
}

bool get_accel(float &x, float &y, float &z) {
    if (qmi.getDataReady()) {
        return qmi.getAccelerometer(x, y, z);
    }
    return false;
}

void set_brightness(uint8_t val) {
    gfx->setBrightness(val);
}

void set_brightness_percentage(uint8_t valPercentage) {
    int value = (int)(255 * valPercentage * 0.01);
    gfx->setBrightness(value);
}

//set volume adjusted for log scaling for finer control
void set_volume_percentage(uint8_t valPercentage) {
    if (es_handle != NULL) {
        uint8_t mappedVolume;
        
        if (valPercentage == 0) {
            mappedVolume = 0;
        } else {
            float normalized = valPercentage / 100.0f;
            float logValue = log10f(1.0f + 9.0f * normalized);
            mappedVolume = (uint8_t)(logValue * 100.0f);
        }
        
        es8311_voice_volume_set(es_handle, mappedVolume, NULL);
        Serial.println("Mapped volume: " + String(mappedVolume));
    }
}