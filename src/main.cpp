#include <Arduino.h>
#include "scr_st77916.h"
// #include <lvgl.h>
// #include <demos/lv_demos.h>
#include <ui.h>
#include <HardwareSerial.h>
#include <esp_now.h>
#include <WiFi.h>
// REPLACE WITH YOUR ESP RECEIVER'S MAC ADDRESS
uint8_t broadcastAddress1[] = { 0x90,0x38,0x0c,0x63,0xef,0x58};
unsigned char  checkbt1 = 0 ;
unsigned char checkbt2 = 0;
unsigned char checkbt3 = 0;

typedef struct test_swich {
  bool swich_1;
  bool swich_2;
  bool swich_3;
} test_swich;
test_swich swichdata;
esp_now_peer_info_t peerInfo;


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void send_data_swich_ON(bool s1,bool s2,bool s3);
void On_relay220VAC(lv_event_t * e);
void Btb_close(lv_event_t * e);
void Pressd_S1(lv_event_t * e);
void Pressd_S2(lv_event_t * e);
void Pressd_S3(lv_event_t * e);


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
void send_data_swich_ON(bool s1,bool s2,bool s3){
  swichdata.swich_1 = s1;
  swichdata.swich_2 = s2;
  swichdata.swich_3 = s3;
  esp_err_t result = esp_now_send(0, (uint8_t *) &swichdata, sizeof(swichdata));
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}

void On_relay220VAC(lv_event_t * e)
{
	// Your code here
}

void Btb_close(lv_event_t * e)
{
	// Your code here
  
   send_data_swich_ON(0,0,0);

}

void Pressd_S1(lv_event_t * e)
{
	// Your code here
  checkbt1++;
  switch (checkbt1)
  {
  case 0:
     send_data_swich_ON(0,NULL,NULL);
    break;
  case 1:
     send_data_swich_ON(1,NULL,NULL);
    break;
  case 2:
     send_data_swich_ON(0,NULL,NULL);
     checkbt1 = 0;
    break; 
  default:
    break;
  }
}

void Pressd_S3(lv_event_t * e)
{
	checkbt3++;
  switch (checkbt3)
  {
  case 0:
     send_data_swich_ON(NULL,NULL,0);
    break;
  case 1:
     send_data_swich_ON(NULL,NULL,1);
    break;
  case 2:
    send_data_swich_ON(NULL,NULL,0);
    checkbt3 = 0;
    break;
  default:
    break;
  }
}

void Pressd_S2(lv_event_t * e)
{
	checkbt2++;
  switch (checkbt2)
  {
  case 0:
     send_data_swich_ON(NULL,0,NULL);
    break;
  case 1:
     send_data_swich_ON(NULL,1,NULL);
    break;
  case 2:
    send_data_swich_ON(NULL,0,NULL);
    checkbt2 = 0;
    break;
  default:
    break;
  }
}
void setup()
{
  delay(200);
  Serial.begin(115200);
  scr_lvgl_init();
  // lv_demo_widgets();
  //lv_demo_benchmark();
  //lv_demo_music();
  ui_init();
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
   
  // register peer
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  // register first peer  
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop()
{
  lv_timer_handler();
  vTaskDelay(5);
  Serial.print("Pao");
}
