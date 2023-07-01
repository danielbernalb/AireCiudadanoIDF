#include <Arduino.h>

//#include "C:\esp\ESP8266_RTOS_SDK\components\esp8266\include\esp_wifi.h"
//#include "C:\esp\ESP8266_RTOS_SDK\components\wpa_supplicant\include\esp_supplicant\esp_wpa2.h"
//#include "C:\esp\ESP8266_RTOS_SDK\components\freertos\include\freertos\FreeRTOS.h"

//#include "C:\esp32\esp-idf\components\esp_wifi\include"

#include "C:\esp32\esp-idf\components\esp_wifi\include\esp_wifi.h"
#include "C:\esp32\esp-idf\components\wpa_supplicant\esp_supplicant\include\esp_wpa2.h"

//#include "freertos/FreeRTOS.h"
#include "C:\esp32\esp-idf\components\freertos\FreeRTOS-Kernel\include\freertos\FreeRTOS.h"
//#include "freertos/task.h"
#include "C:\esp32\esp-idf\components\freertos\FreeRTOS-Kernel\include\freertos\task.h"
//#include "freertos/event_groups.h"
#include "C:\esp32\esp-idf\components\freertos\FreeRTOS-Kernel\include\freertos\event_groups.h"
//#include "esp_wifi.h"
//#include "esp_wpa2.h"
//#include "esp_event.h"

//#include "C:\esp32\esp-idf\components\esp_netif\include\esp_netif.h"

#include "C:\esp32\esp-idf\components\esp_event\include\esp_event.h"
//#include "esp_log.h"
#include "C:\esp32\esp-idf\components\log\include\esp_log.h"
//#include "esp_system.h"
#include "C:\esp32\esp-idf\components\esp_system\include\esp_system.h"
//#include "nvs_flash.h"
#include "C:\esp32\esp-idf\components\nvs_flash\include\nvs_flash.h"
//#include "esp_netif.h"

//#include "esp_err.h"
#include "C:\esp32\esp-idf\components\esp_common\include\esp_err.h"
//#include "esp_wifi_types.h"
#include "C:\esp32\esp-idf\components\esp_wifi\include\esp_wifi_types.h"
//#include "esp_event.h"
//#include "C:\esp32\esp-idf\components\esp_event\include\esp_event.h
//#include "esp_private/esp_wifi_private.h"
#include "C:\esp32\esp-idf\components\esp_wifi\include\esp_private\esp_wifi_private.h"
//#include "esp_wifi_default.h"
#include "C:\esp32\esp-idf\components\esp_wifi\include\esp_wifi_default.h"

////////////////////////////////

//#include "C:\esp32\esp-idf\components\esp_hw_support\include\esp_private\adc_share_hw_ctrl.h"
//#include "C:\esp32\esp-idf\components\esp_wifi\src\wifi_init.c"

///////////////////////////////

//#define EXAMPLE_WIFI_SSID "SED CORPORATIVO"
//#define EXAMPLE_EAP_ID "wiedctabora"
//#define EXAMPLE_EAP_USERNAME "wiedctabora"
//#define EXAMPLE_EAP_PASSWORD "OZ4\"Xbk0"

#define EXAMPLE_WIFI_SSID "TPwpa2"
#define EXAMPLE_EAP_ID "prueba1"
#define EXAMPLE_EAP_USERNAME "prueba1"
#define EXAMPLE_EAP_PASSWORD "daniel2022"

#define WIFI_INIT_CONFDEF() { \
    .osi_funcs = &g_wifi_osi_funcs, \
    .wpa_crypto_funcs = g_wifi_default_wpa_crypto_funcs, \
    .static_rx_buf_num = CONFIG_ESP32_WIFI_STATIC_RX_BUFFER_NUM,\
    .dynamic_rx_buf_num = CONFIG_ESP32_WIFI_DYNAMIC_RX_BUFFER_NUM,\
    .tx_buf_type = CONFIG_ESP32_WIFI_TX_BUFFER_TYPE,\
    .static_tx_buf_num = WIFI_STATIC_TX_BUFFER_NUM,\
    .dynamic_tx_buf_num = WIFI_DYNAMIC_TX_BUFFER_NUM,\
    .cache_tx_buf_num = WIFI_CACHE_TX_BUFFER_NUM,\
    .csi_enable = WIFI_CSI_ENABLED,\
    .ampdu_rx_enable = WIFI_AMPDU_RX_ENABLED,\
    .ampdu_tx_enable = WIFI_AMPDU_TX_ENABLED,\
    .amsdu_tx_enable = WIFI_AMSDU_TX_ENABLED,\
    .nvs_enable = WIFI_NVS_ENABLED,\
    .nano_enable = WIFI_NANO_FORMAT_ENABLED,\
    .rx_ba_win = WIFI_DEFAULT_RX_BA_WIN,\
    .wifi_task_core_id = WIFI_TASK_CORE_ID,\
    .beacon_max_len = WIFI_SOFTAP_BEACON_MAX_LEN, \
    .mgmt_sbuf_num = WIFI_MGMT_SBUF_NUM, \
    .feature_caps = g_wifi_feature_caps, \
    .sta_disconnected_pm = WIFI_STA_DISCONNECTED_PM_ENABLED,  \
    .espnow_max_encrypt_num = CONFIG_ESP_WIFI_ESPNOW_MAX_ENCRYPT_NUM, \
    .magic = WIFI_INIT_CONFIG_MAGIC\
}


//* FreeRTOS event group to signal when we are connected & ready to make a request */
static EventGroupHandle_t wifi_event_group;

/* esp netif object representing the WIFI station */
static esp_netif_t *sta_netif = NULL;

/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
const int CONNECTED_BIT = BIT0;

static const char *TAG = "example";

static void event_handler(void* arg, esp_event_base_t event_base,
                          int32_t event_id, void* event_data)
{
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    esp_wifi_connect();
    ESP_LOGI(TAG, "WIFI_EVENT_STA_START");
  } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
    esp_wifi_connect();
    xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
    ESP_LOGI(TAG, "WIFI_EVENT_STA_DISCONNECTED");
  } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
    ESP_LOGI(TAG, "IP_EVENT_STA_GOT_IP");
  }
}

void initialise_wifi()
{
  ESP_ERROR_CHECK(esp_netif_init());
  wifi_event_group = xEventGroupCreate();
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  sta_netif = esp_netif_create_default_wifi_sta();
  assert(sta_netif);
  Serial.println("Test 6");  
  wifi_init_config_t cfg = WIFI_INIT_CONFDEF();
  Serial.println("Test 7");
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  Serial.println("Test 8");
  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
  Serial.println("Test 9");
  ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));
  Serial.println("Test 10");
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  Serial.println("Test 11");
  
  wifi_config_t conf;
  
  ESP_LOGI(TAG, "SSID: %s", EXAMPLE_WIFI_SSID);
  ESP_LOGI(TAG, "Identity: %s", EXAMPLE_EAP_ID);
  ESP_LOGI(TAG, "Password: %s", EXAMPLE_EAP_PASSWORD);
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &conf));
  esp_wifi_sta_wpa2_ent_clear_identity();
  esp_wifi_sta_wpa2_ent_clear_username();
  esp_wifi_sta_wpa2_ent_clear_password();
  esp_wifi_sta_wpa2_ent_clear_new_password();
  esp_wifi_sta_wpa2_ent_clear_ca_cert();
  esp_wifi_sta_wpa2_ent_clear_cert_key();
  //    esp_wifi_sta_wpa2_ent_set_disable_time_check(false);
  Serial.println("Test 12");

  ESP_ERROR_CHECK(esp_wifi_sta_wpa2_ent_set_identity((uint8_t*)EXAMPLE_EAP_ID, strlen(EXAMPLE_EAP_ID)));
  ESP_ERROR_CHECK(esp_wifi_sta_wpa2_ent_set_username((uint8_t*)EXAMPLE_EAP_USERNAME, strlen(EXAMPLE_EAP_USERNAME)));
  ESP_ERROR_CHECK(esp_wifi_sta_wpa2_ent_set_password((uint8_t*)EXAMPLE_EAP_PASSWORD, strlen(EXAMPLE_EAP_PASSWORD)));
  ESP_ERROR_CHECK(esp_wifi_sta_wpa2_ent_enable());
  ESP_ERROR_CHECK(esp_wifi_start());
  Serial.println("Wifi start");
  Serial.println("wifi_init_sta finished.");

}

static void wpa2_enterprise_example_task(void *pvParameters)
{
  esp_netif_ip_info_t ip;
  memset(&ip, 0, sizeof(esp_netif_ip_info_t));
  vTaskDelay(2000 / portTICK_PERIOD_MS);

  while (1) {
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    if (esp_netif_get_ip_info(sta_netif, &ip) == 0) {
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
//void app_main()
void loop()
{
  xTaskCreate(&wpa2_enterprise_example_task, "wpa2_enterprise_example_task", 4096, NULL, 5, NULL);
  ESP_LOGI(TAG, "main");
}
