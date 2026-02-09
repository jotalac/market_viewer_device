#include "WifiConfig.h"
#include <WiFiManager.h>
#include <Preferences.h>

static WiFiManager wifiManager;
static Preferences preferences;
static WifiConfig wifiConfig;

// Custom parameters
static WiFiManagerParameter* backendUrlParam;
static WiFiManagerParameter* deviceHashParam;

String wifiSSID = "market_viewer_setup";
String wifiPassword = "marketViewer321";
String defaultBackendUrl = "http://localhost:8080/api";



void init_wifi_config() {
    // Load saved configuration
    preferences.begin("wifi_config", true);  // Read-only
    
    String savedUrl = preferences.getString("backendUrl", defaultBackendUrl);
    String savedName = preferences.getString("deviceHash", "");
    wifiConfig.configured = preferences.getBool("configured", false);
    
    strncpy(wifiConfig.backendUrl, savedUrl.c_str(), sizeof(wifiConfig.backendUrl) - 1);
    strncpy(wifiConfig.deviceHash, savedName.c_str(), sizeof(wifiConfig.deviceHash) - 1);
    
    preferences.end();
    
    Serial.println("WiFi Config initialized");
    Serial.println("Configured: " + String(wifiConfig.configured ? "Yes" : "No"));
    
    // Try to auto-connect with saved credentials
    WiFi.mode(WIFI_STA);
    WiFi.begin();
    
    // Wait a bit for connection
    int timeout = 10;
    while (WiFi.status() != WL_CONNECTED && timeout > 0) {
        delay(500);
        Serial.print(".");
        timeout--;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected!");
        Serial.println("IP: " + WiFi.localIP().toString());
    } else {
        Serial.println("\nWiFi not connected. Use portal to configure.");
    }
}

void save_config_callback() {
    Serial.println("Saving configuration...");
    
    // Get values from parameters
    strncpy(wifiConfig.backendUrl, backendUrlParam->getValue(), sizeof(wifiConfig.backendUrl) - 1);
    strncpy(wifiConfig.deviceHash, deviceHashParam->getValue(), sizeof(wifiConfig.deviceHash) - 1);
    wifiConfig.configured = true;
    
    // Save to flash
    preferences.begin("wifi_config", false);
    preferences.putString("backendUrl", wifiConfig.backendUrl);
    preferences.putString("deviceHash", wifiConfig.deviceHash);
    preferences.putBool("configured", true);
    preferences.end();
    
    Serial.println("Config saved!");
    Serial.println("Backend URL: " + String(wifiConfig.backendUrl));
    Serial.println("Device Name: " + String(wifiConfig.deviceHash));
}

bool start_wifi_portal() {
    Serial.println("Starting WiFi configuration portal...");
    
    // Create custom parameters
    backendUrlParam = new WiFiManagerParameter("backend_url", "Backend URL", wifiConfig.backendUrl, 256);
    deviceHashParam = new WiFiManagerParameter("device_hash", "Device hash", wifiConfig.deviceHash, 256);
    
    // Add parameters to WiFiManager
    wifiManager.addParameter(backendUrlParam);
    wifiManager.addParameter(deviceHashParam);
    
    // Set save callback
    wifiManager.setSaveConfigCallback(save_config_callback);
    
    // Configure portal
    wifiManager.setConfigPortalTimeout(180);  // 3 minutes timeout
    wifiManager.setAPClientCheck(true);
    
    // Start portal - this blocks until configured or timeout
    bool connected = wifiManager.startConfigPortal(wifiSSID.c_str(), wifiPassword.c_str());
    
    // Clean up
    delete backendUrlParam;
    delete deviceHashParam;
    
    if (connected) {
        Serial.println("WiFi configured successfully!");
        return true;
    } else {
        Serial.println("WiFi configuration failed or timed out.");
        return false;
    }
}

bool is_wifi_connected() {
    return WiFi.status() == WL_CONNECTED;
}

const char* get_connected_ssid() {
    static String ssid;
    if (WiFi.status() == WL_CONNECTED) {
        ssid = WiFi.SSID();
        return ssid.c_str();
    }
    return "Not Connected";
}

String get_ip_address() {
    if (WiFi.status() == WL_CONNECTED) {
        return WiFi.localIP().toString();
    }
    return "0.0.0.0";
}

WifiConfig get_device_config() {
    return wifiConfig;
}

const char* get_backend_url() {
    return wifiConfig.backendUrl;
}

const char* get_device_hash() {
    return wifiConfig.deviceHash;
}
    
void reset_wifi_config() {
    Serial.println("Resetting WiFi configuration...");
    
    // Clear preferences
    preferences.begin("wifi_config", false);
    preferences.clear();
    preferences.end();
    
    // Clear WiFi credentials
    wifiManager.resetSettings();
    
    // Reset struct
    memset(&wifiConfig, 0, sizeof(wifiConfig));
    strcpy(wifiConfig.backendUrl, defaultBackendUrl.c_str());
    strcpy(wifiConfig.deviceHash, "");
    wifiConfig.configured = false;
    
    Serial.println("Configuration reset complete!");
}