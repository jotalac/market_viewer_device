#include "HttpRequestManager.h"
#include "WifiConfig.h"
#include <HTTPClient.h>

static HTTPClient http;

String get_base_url() {
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

    url += "hardware/" + String(deviceHash);

    return url;
}

String fetch_data(String url) {
    if (!is_wifi_connected()) {
        Serial.println("Cannot fetch data - WiFi not connected");
        return {};
    }
    
    Serial.println("Fetching screens from: " + url);
    
    http.setTimeout(15000);  // 15 second timeout
    http.begin(url);
    
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


// getting all screen info
String create_fetch_screens_url() {
   String url = get_base_url();
    url += "/screen";
    return url;
}

String fetch_screens() {
    String url = create_fetch_screens_url();
    return fetch_data(url);
}


//getting single screen data
String create_fetch_screen_data_url(int screenPosition) {
    String url = get_base_url();
    url += "/screen/" + String(screenPosition) + "/data";
    return url;
}

String fetch_screen_data(int screenPosition) {
    String url = create_fetch_screen_data_url(screenPosition);
    return fetch_data(url);
}