#include "HttpRequestManager.h"
#include "WifiConfig.h"
#include <HTTPClient.h>

static HTTPClient http;

String create_fetch_screens_url() {
    const char* backendUrl = get_backend_url();
    const char* deviceHash = get_device_hash();
    
    //check if values are set
    if (strlen(backendUrl) == 0 || strlen(deviceHash) == 0) {
        Serial.println("Backend URL or device hash not configured");
        return "";
    }
    
    
    String url = String(backendUrl);

    //ensure user added http
    if (!url.startsWith("http://") && !url.startsWith("https://")) {
        url = "http://" + url;
    }

    // Ensure URL ends with /
    if (!url.endsWith("/")) {
        url += "/";
    }
    
    url += "hardware/" + String(deviceHash) + "/screen";
    
    return url;
}


String fetch_screens_from_backend() {
    if (!is_wifi_connected()) {
        Serial.println("Cannot fetch screens - WiFi not connected");
        return {};
    }
    
    String url = create_fetch_screens_url();
    Serial.println("Fetching screens from: " + url);
    
    http.begin(url);
    http.setTimeout(10000);  // 10 second timeout
    
    int httpCode = http.GET();
    
    if (httpCode != HTTP_CODE_OK) {
        Serial.println("HTTP error: " + String(httpCode));
        http.end();
        return {};
    }
    
    String payload = http.getString();
    http.end();
    
    Serial.println("Received: " + payload);
    
    return payload;
}