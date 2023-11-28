/**
 * Minimal ESP-IDF (no Arduino) example for HumanESPHTTP
 * 
 * This example is released under CC0 1.0 Universal
 * (C) 2023 Uli Koehler
 */
#include <cstdio>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <HTTPServer.hpp>

#include <esp_wifi.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <esp_netif.h>
#include <esp_event.h>


extern "C" {
    void app_main(void);
}

HTTPServer http;

// Example handler
static const httpd_uri_t helloWorldHandler = {
    .uri       = "/api/hello-world",
    .method    = HTTP_GET,
    .handler   = [](httpd_req_t *request) {
        httpd_resp_set_type(request, "text/plain");
        httpd_resp_sendstr(request, "Hello World!");

        return ESP_OK;
    }
};

/**
 * This is called once the Wifi is connected and has acquired an IP address
 */
static void OnWifiConnected() {
    // Start HTTP server
    http.StartServer();
    http.RegisterHandler(&helloWorldHandler);
}


static void NetworkEventHandler(void* arg, esp_event_base_t event_base,
                          int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        ESP_LOGI("wifi", "Retrying to connect to Wifi...");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI("wifi", "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        // Call sub-handler
        OnWifiConnected();
    }
}

void InitNVS() {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void InitWifi() {

    // Initialize TCP/IP network interface (required for Wi-Fi)
    ESP_ERROR_CHECK(esp_netif_init());

    // Initialize the event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    // Initialize Wi-Fi
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Set Wi-Fi to station mode
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    // Configure the Wi-Fi connection
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "MyWifi",
            .password = "abc123"
        }
    };
    // Register event handler
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &NetworkEventHandler,
                                                        NULL,
                                                        NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &NetworkEventHandler,
                                                        NULL,
                                                        NULL));

    // Set Wi-Fi configuration and start Wi-Fi
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void app_main() {
    InitNVS();
    InitWifi();

    while(true) {
        printf("Hello PlatformIO!\n");
        // Wait for one second
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
