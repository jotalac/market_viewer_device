#include "HttpRequestManager.h"
#include "WifiConfig.h"
#include <HTTPClient.h>

// static HTTPClient http;

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

bool fetch_data(String url, JsonDocument& doc) {
if (!is_wifi_connected()) return false;
    
    HTTPClient http; 
    http.setTimeout(15000); 

    //force HTTP/1.0 to disable chunked transfer encoding.
    // This guarantees the stream will contain pure JSON without hex headers.
    http.useHTTP10(true); 
    
    http.begin(url);
    int httpCode = http.GET();
    
    if (httpCode == HTTP_CODE_OK) {
        // Parse directly from the clean stream
        DeserializationError error = deserializeJson(doc, http.getStream());
        
        http.end(); 
        
        if (error) {
            Serial.println("JSON parse error: " + String(error.c_str()));
            return false;
        }
        return true;
    }
    
    Serial.println("HTTP error: " + String(httpCode));
    http.end();
    return false;
}


// getting all screen info
String create_fetch_screens_url() {
   String url = get_base_url();
    url += "/screen";
    return url;
}

bool fetch_screens(JsonDocument& doc) {
    String url = create_fetch_screens_url();
    return fetch_data(url, doc);
}


//getting single screen data
String create_fetch_screen_data_url(int screenPosition) {
    String url = get_base_url();
    url += "/screen/" + String(screenPosition) + "/data";
    return url;
}

bool fetch_screen_data(int screenPosition, JsonDocument& doc) {
    String url = create_fetch_screen_data_url(screenPosition);
    return fetch_data(url, doc);
}