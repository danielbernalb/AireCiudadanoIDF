#define ESP32 true
#define ESP8266 false

#if ESP8266

#include  <ESP8266WiFi.h>

extern "C" {
#include "user_interface.h"
#include "wpa2_enterprise.h"
#include "c_types.h"
}

// SSID to connect to

 /*
char ssid[] = "SED CORPORATIVO";
char username[] = "wiedctabora";
char identity[] = "wiedctabora";
char password[] = "OZ4\"Xbk0";
 */

// /*
//char ssid[] = "dd-wrt";
char ssid[] = "TPwpa2";
char username[] = "prueba1";
char identity[] = "prueba1";
char password[] = "daniel2022";
// */

//wifi_active_scan_time_t 1000;

// uint8_t target_esp_mac[6] = {0x24, 0x0a, 0xc4, 0x9a, 0x58, 0x28};
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.setDebugOutput(true);
  enable_wifi_enterprise_patch();
  disable_extra4k_at_link_time();
  Serial.println("");
  Serial.println("");
  Serial.print(F("ESP CoreVersion: "));
  Serial.println(ESP.getCoreVersion());
  Serial.printf("SDK version: %s\n", system_get_sdk_version());
  Serial.printf("Free Heap: %4d\n",ESP.getFreeHeap());

  Serial.print("ssid: ");
  Serial.println(ssid);
  Serial.print("identidad: ");
  Serial.println(identity);
  Serial.print("password: ");
  Serial.println(password);

//  Serial.println(password);
  WiFi.mode(WIFI_STA);
  delay(100);
  // Setting ESP into STATION mode only (no AP mode or dual mode)
  wifi_set_opmode(STATION_MODE);

  struct station_config wifi_config;

  memset(&wifi_config, 0, sizeof(wifi_config));
  strcpy((char*)wifi_config.ssid, ssid);
  strcpy((char*)wifi_config.password, password);

  wifi_station_set_config(&wifi_config);
//  wifi_set_macaddr(STATION_IF,target_esp_mac);
  

  wifi_station_set_wpa2_enterprise_auth(1);

  // Clean up to be sure no old data is still inside
  wifi_station_clear_cert_key();
  wifi_station_clear_enterprise_ca_cert();
  wifi_station_clear_enterprise_identity();
  wifi_station_clear_enterprise_username();
  wifi_station_clear_enterprise_password();
  wifi_station_clear_enterprise_new_password();
  
  wifi_station_set_enterprise_identity((uint8*)identity, strlen(identity));
  wifi_station_set_enterprise_username((uint8*)username, strlen(username));
  wifi_station_set_enterprise_password((uint8*)password, strlen((char*)password));

  
  wifi_station_connect();
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Serial.println("WiFi connected");
  Serial.println(WiFi.RSSI());
  delay(1000);
}

#endif

#if ESP32

#include <WiFi.h>
#include "esp_wpa2.h"

#define WIFI_SSID "dd-wrt"
#define EAP_USERNAME "prueba1"
#define EAP_PASSWORD "daniel2022"

void setup() {
  Serial.begin(115200);
  Serial.println("INIT");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  Serial.println(F("Attempting to authenticate using WPA2 Enterprise"));
    Serial.print(F("Identity: "));
    Serial.println(EAP_USERNAME);
    Serial.print(F("Password: "));
    Serial.println(EAP_PASSWORD);
    
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
  // put your main code here, to run repeatedly:
  Serial.println(WiFi.RSSI());
  delay(1000);
}


#endif