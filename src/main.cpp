#include <Arduino.h>

#include <string.h>
#include <stdlib.h>

// #include "freertos/FreeRTOS.h"
#include "C:\esp\ESP8266_RTOS_SDK\components\freertos\include\freertos\FreeRTOS.h"
// #include "freertos/task.h"
#include "C:\esp\ESP8266_RTOS_SDK\components\freertos\include\freertos\task.h"

// #include "freertos/event_groups.h"
#include "C:\esp\ESP8266_RTOS_SDK\components\freertos\include\freertos\event_groups.h"
// #include "esp_wifi.h"
#include "C:\esp\ESP8266_RTOS_SDK\components\esp8266\include\esp_wifi.h"
// #include "esp_wpa2.h"
#include "C:\esp\ESP8266_RTOS_SDK\components\wpa_supplicant\include\esp_supplicant\esp_wpa2.h"
// #include "esp_event.h"
#include "C:\esp\ESP8266_RTOS_SDK\components\esp_event\include\esp_event.h"
// #include "esp_log.h"
#include "C:\esp\ESP8266_RTOS_SDK\components\log\include\esp_log.h"
// #include "esp_system.h"
#include "C:\esp\ESP8266_RTOS_SDK\components\esp8266\include\esp_system.h"
// #include "nvs_flash.h"
#include "C:\esp\ESP8266_RTOS_SDK\components\nvs_flash\include\nvs_flash.h"
// #include "tcpip_adapter.h"
#include "C:\esp\ESP8266_RTOS_SDK\components\tcpip_adapter\include\tcpip_adapter.h"
// #include "esp_event_loop.h"
#include "C:\esp\ESP8266_RTOS_SDK\components\esp_event\include\esp_event_loop.h"

/* The examples use simple WiFi configuration that you can set via
   'make menuconfig'.

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"

   You can choose EAP method via 'make menuconfig' according to the
   configuration of AP.
*/

// #define EXAMPLE_WIFI_SSID "SED CORPORATIVO"
// #define EXAMPLE_EAP_ID "wiedctabora"
// #define EXAMPLE_EAP_USERNAME "wiedctabora"
// #define EXAMPLE_EAP_PASSWORD "OZ4\"Xbk0"

#define EXAMPLE_WIFI_SSID "TPwpa2"
#define EXAMPLE_EAP_ID "prueba1"
#define EXAMPLE_EAP_USERNAME "prueba1"
#define EXAMPLE_EAP_PASSWORD "daniel2022"

/* FreeRTOS event group to signal when we are connected & ready to make a request */
static EventGroupHandle_t wifi_event_group;

/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
#define WIFI_CONNECTED_BIT BIT0

static const char *TAG = "example";

static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data)
{

  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
  {
    esp_wifi_connect();
    ESP_LOGI(TAG, "WIFI_EVENT_STA_START");
  }
  else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
  {
    system_event_sta_disconnected_t *event = (system_event_sta_disconnected_t *)event_data;

    ESP_LOGI(TAG, "WIFI_EVENT_STA_DISCONNECTED");
  }
  else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
  {
    xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    ESP_LOGI(TAG, "IP_EVENT_STA_GOT_IP");
  }
}

// static void initialise_wifi(void)
void initialise_wifi()
{
  tcpip_adapter_init();
  wifi_event_group = xEventGroupCreate();
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
  ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  //  wifi_config_t wifi_config = {
  //      .sta = {
  //          .ssid = EXAMPLE_WIFI_SSID,
  //      },
  //  };
  ESP_LOGI(TAG, "SSID: %s", EXAMPLE_WIFI_SSID);
  ESP_LOGI(TAG, "Identity: %s", EXAMPLE_EAP_ID);
  ESP_LOGI(TAG, "Password: %s", EXAMPLE_EAP_PASSWORD);
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
  esp_wifi_sta_wpa2_ent_clear_identity();
  esp_wifi_sta_wpa2_ent_clear_username();
  esp_wifi_sta_wpa2_ent_clear_password();
  esp_wifi_sta_wpa2_ent_clear_new_password();
  esp_wifi_sta_wpa2_ent_clear_ca_cert();
  esp_wifi_sta_wpa2_ent_clear_cert_key();
  esp_wifi_sta_wpa2_ent_set_disable_time_check(false);

  ESP_ERROR_CHECK(esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EXAMPLE_EAP_ID, strlen(EXAMPLE_EAP_ID)));
  ESP_ERROR_CHECK(esp_wifi_sta_wpa2_ent_enable());
  ESP_ERROR_CHECK(esp_wifi_start());
  // Serial.println("Wifi start");
  ESP_LOGI(TAG, "Wifi start");
  ESP_LOGI(TAG, "wifi_init_sta finished.");
}

static void wpa2_enterprise_example_task(void *pvParameters)
{
  tcpip_adapter_ip_info_t ip;
  memset(&ip, 0, sizeof(tcpip_adapter_ip_info_t));
  vTaskDelay(2000 / portTICK_PERIOD_MS);

  vTaskDelay(2000 / portTICK_PERIOD_MS);

  while (1)
  {
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    if (esp_netif_get_ip_info(sta_netif, &ip) == 0)
    {
      ESP_LOGI(TAG, "~~~~~~~~~~~");
      ESP_LOGI(TAG, "IP:" IPSTR, IP2STR(&ip.ip));
      ESP_LOGI(TAG, "MASK:" IPSTR, IP2STR(&ip.netmask));
      ESP_LOGI(TAG, "GW:" IPSTR, IP2STR(&ip.gw));
      ESP_LOGI(TAG, "~~~~~~~~~~~");
      //  Serial.println("task");
      ESP_LOGI(TAG, "task");
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("INIT");
  Serial.println(F("Attempting to authenticate using WPA2 Enterprise..."));
  Serial.print("SSID: ");
  Serial.println(EXAMPLE_WIFI_SSID);
  Serial.print(F("Identity: "));
  Serial.println(EXAMPLE_EAP_ID);
  Serial.print(F("Password: "));
  Serial.println(EXAMPLE_EAP_PASSWORD);

  static EventGroupHandle_t wifi_event_group;

  initialise_wifi();
}
// void app_main()
void loop()
{
  xTaskCreate(&wpa2_enterprise_example_task, "wpa2_enterprise_example_task", 4096, NULL, 5, NULL);
  ESP_LOGI(TAG, "main");
}