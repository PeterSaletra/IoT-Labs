/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

 #include <Arduino.h>

 #include <WiFi.h>
 #include <WiFiMulti.h>
 
 #include <HTTPClient.h>
 
 #define USE_SERIAL Serial
 
 WiFiMulti wifiMulti;
 
 /*
 const char* ca = \
 "-----BEGIN CERTIFICATE-----\n" \
 "MIIEkjCCA3qgAwIBAgIQCgFBQgAAAVOFc2oLheynCDANBgkqhkiG9w0BAQsFADA/\n" \
 "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
 "DkRTVCBSb290IENBIFgzMB4XDTE2MDMxNzE2NDA0NloXDTIxMDMxNzE2NDA0Nlow\n" \
 "SjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxIzAhBgNVBAMT\n" \
 "GkxldCdzIEVuY3J5cHQgQXV0aG9yaXR5IFgzMIIBIjANBgkqhkiG9w0BAQEFAAOC\n" \
 "AQ8AMIIBCgKCAQEAnNMM8FrlLke3cl03g7NoYzDq1zUmGSXhvb418XCSL7e4S0EF\n" \
 "q6meNQhY7LEqxGiHC6PjdeTm86dicbp5gWAf15Gan/PQeGdxyGkOlZHP/uaZ6WA8\n" \
 "SMx+yk13EiSdRxta67nsHjcAHJyse6cF6s5K671B5TaYucv9bTyWaN8jKkKQDIZ0\n" \
 "Z8h/pZq4UmEUEz9l6YKHy9v6Dlb2honzhT+Xhq+w3Brvaw2VFn3EK6BlspkENnWA\n" \
 "a6xK8xuQSXgvopZPKiAlKQTGdMDQMc2PMTiVFrqoM7hD8bEfwzB/onkxEz0tNvjj\n" \
 "/PIzark5McWvxI0NHWQWM6r6hCm21AvA2H3DkwIDAQABo4IBfTCCAXkwEgYDVR0T\n" \
 "AQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMCAYYwfwYIKwYBBQUHAQEEczBxMDIG\n" \
 "CCsGAQUFBzABhiZodHRwOi8vaXNyZy50cnVzdGlkLm9jc3AuaWRlbnRydXN0LmNv\n" \
 "bTA7BggrBgEFBQcwAoYvaHR0cDovL2FwcHMuaWRlbnRydXN0LmNvbS9yb290cy9k\n" \
 "c3Ryb290Y2F4My5wN2MwHwYDVR0jBBgwFoAUxKexpHsscfrb4UuQdf/EFWCFiRAw\n" \
 "VAYDVR0gBE0wSzAIBgZngQwBAgEwPwYLKwYBBAGC3xMBAQEwMDAuBggrBgEFBQcC\n" \
 "ARYiaHR0cDovL2Nwcy5yb290LXgxLmxldHNlbmNyeXB0Lm9yZzA8BgNVHR8ENTAz\n" \
 "MDGgL6AthitodHRwOi8vY3JsLmlkZW50cnVzdC5jb20vRFNUUk9PVENBWDNDUkwu\n" \
 "Y3JsMB0GA1UdDgQWBBSoSmpjBH3duubRObemRWXv86jsoTANBgkqhkiG9w0BAQsF\n" \
 "AAOCAQEA3TPXEfNjWDjdGBX7CVW+dla5cEilaUcne8IkCJLxWh9KEik3JHRRHGJo\n" \
 "uM2VcGfl96S8TihRzZvoroed6ti6WqEBmtzw3Wodatg+VyOeph4EYpr/1wXKtx8/\n" \
 "wApIvJSwtmVi4MFU5aMqrSDE6ea73Mj2tcMyo5jMd6jmeWUHK8so/joWUoHOUgwu\n" \
 "X4Po1QYz+3dszkDqMp4fklxBwXRsW10KXzPMTZ+sOPAveyxindmjkW8lGy+QsRlG\n" \
 "PfZ+G6Z6h7mjem0Y+iWlkYcV4PIWL1iwBi8saCbGS5jN2p8M+X+Q7UNKEkROb3N6\n" \
 "KOqkqm57TH2H3eDJAkSnh6/DNFu0Qg==\n" \
 "-----END CERTIFICATE-----\n";
 */
 
 double extractTemperature(const std::string& json) {
  std::string key = "\"temperature_2m\":";
  size_t pos = json.find(key);
  
  if (pos != std::string::npos) {
      pos += key.length();  // Move to the position after the key
      size_t endPos = json.find(',', pos);  // Find the end of the value
      
      if (endPos == std::string::npos) {  
          endPos = json.find('}', pos);  // Handle case where it's the last element
      }
      
      std::string tempValue = json.substr(pos, endPos - pos);  // Extract the value
      return std::stod(tempValue);  // Convert to double
  }
  
  return -1; // Return -1 if not found (error case)
}

 void setup() {
 
   USE_SERIAL.begin(115200);
 
   USE_SERIAL.println();
   USE_SERIAL.println();
   USE_SERIAL.println();
 
   for (uint8_t t = 4; t > 0; t--) {
     USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
     USE_SERIAL.flush();
     delay(1000);
   }
 
   wifiMulti.addAP("PK", "12345678");
 }
 
 void loop() {
   // wait for WiFi connection
   if ((wifiMulti.run() == WL_CONNECTED)) {
 
     HTTPClient http;
 
     USE_SERIAL.print("[HTTP] begin...\n");
     // configure traged server and url
     //http.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS
     http.begin("https://api.open-meteo.com/v1/forecast?latitude=50.06&longitude=19.94&current=temperature_2m");  //HTTP
 
     USE_SERIAL.print("[HTTP] GET...\n");
     // start connection and send HTTP header
     int httpCode = http.GET();
 
     // httpCode will be negative on error
     if (httpCode > 0) {
       // HTTP header has been send and Server response header has been handled
       USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
 
       // file found at server
       if (httpCode == HTTP_CODE_OK) {
         String payload = http.getString();
         USE_SERIAL.println(extractTemperature(payload.c_str()));
       }
     } else {
       USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
     }
 
     http.end();
   }
 
   delay(5000);
 }

