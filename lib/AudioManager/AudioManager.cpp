#include "AudioManager.h"
#include "HardwareDriver.h"
#include <math.h>
#include "sounds/bop_sound.h"
#include "sounds/test_sound.h"

#define SAMPLE_RATE 16000

static bool audio_ready = false;

void init_audio_manager() {
    if (es_handle != NULL) {
        audio_ready = true;
        Serial.println("AudioManager ready!");
    } else {
        Serial.println("AudioManager: ES8311 not available!");
    }
}

void play_beep(uint16_t frequency, uint16_t duration_ms) {
    if (!audio_ready) {
        Serial.println("Audio not ready!");
        return;
    }
    
    int num_samples = (SAMPLE_RATE * duration_ms) / 1000;
    float samples_per_cycle = (float)SAMPLE_RATE / frequency;
    
    for (int i = 0; i < num_samples; i++) {
        // Generate sine wave
        float angle = (2.0f * M_PI * i) / samples_per_cycle;
        int16_t sample = (int16_t)(sinf(angle) * 16000);  // ~50% amplitude
        
        // Write stereo sample (left and right)
        int16_t stereo_sample[2] = {sample, sample};
        i2s.write((uint8_t*)stereo_sample, sizeof(stereo_sample));
    }
}

void play_volume_beep() {
    play_beep(800, 80);
}

void play_click() {
    play_beep(1200, 30);
}

void play_sound(const int16_t* samples, uint32_t num_samples, bool stereo) {
    if (!audio_ready) {
        Serial.println("Audio not ready!");
        return;
    }
    
    if (stereo) {
        // Stereo: samples already interleaved L,R,L,R...
        i2s.write((uint8_t*)samples, num_samples * sizeof(int16_t));
    } else {
        // Mono: duplicate each sample for L and R channels
        for (uint32_t i = 0; i < num_samples; i++) {
            int16_t sample = pgm_read_word(&samples[i]);
            int16_t stereo_sample[2] = {sample, sample};
            i2s.write((uint8_t*)stereo_sample, sizeof(stereo_sample));
        }
    }
}

void play_bop() {
    play_sound(bop_sound, bop_sound_len, false);
}
