#include <Arduino.h>

#include "C:\Users\DBB\.platformio\packages\framework-arduinoespressif32\libraries\WiFi\src\WiFi.h"
#include "C:\Users\DBB\.platformio\packages\framework-arduinoespressif32\tools\sdk\esp32\include\wpa_supplicant\esp_supplicant\include\esp_wpa2.h"

//#define WIFI_SSID "dd-wrt"
#define WIFI_SSID "TPwpa2"
#define EAP_USERNAME "prueba1"
#define EAP_PASSWORD "daniel2022"

//#define WIFI_SSID "SED CORPORATIVO"
//#define EAP_USERNAME "wfpalestina2"
//#define EAP_PASSWORD "O2p9Cop02o22*+"

void setup() {
  Serial.begin(115200);
  Serial.println("INIT");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  Serial.println(F("Attempting to authenticate using WPA2 Enterprise..."));
    Serial.print("SSID: ");
    Serial.println(WIFI_SSID);
    Serial.print(F("Identity: "));
    Serial.println(EAP_USERNAME);
    Serial.print(F("Password: "));
    Serial.println(EAP_PASSWORD);

    //esp_wifi_sta_wpa2_ent_set_ca_cert((uint8_t *)caCert, caCertLen); 

    esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_USERNAME, strlen(EAP_USERNAME));         // provide identity
    esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_USERNAME, strlen(EAP_USERNAME));         // provide username --> identity and username is same
    esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD)); // provide password
    esp_wifi_sta_wpa2_ent_enable();

  WiFi.begin(WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
}