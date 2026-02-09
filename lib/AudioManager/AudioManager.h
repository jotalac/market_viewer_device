#pragma once
#include <Arduino.h>

// Initialize audio manager (call after init_hardware())
void init_audio_manager();

// Play a simple beep tone
void play_beep(uint16_t frequency = 800, uint16_t duration_ms = 100);

// Play volume feedback beep (uses current volume level)
void play_volume_beep();

// Play a click sound (shorter, higher pitch)
void play_click();

// Play raw PCM audio from flash
void play_sound(const int16_t* samples, uint32_t num_samples, bool stereo = false);

// Play notification sound
void play_bop();