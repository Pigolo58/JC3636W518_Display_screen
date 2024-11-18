#include <esp_now.h>
#include <WiFi.h>

//Structure example to receive data
//Must match the sender structure
//////////// you can config pin for ESP32//////////////
const int LED_S1 = 19;
const int LED_S2 = 18;
const int LED_S3 = 5;
///////////////////////////////////////////////

typedef struct test_dataswich {
  bool s1;
  bool s2;
  bool s3;
} test_dataswich;

//Create a struct_message called myData
test_dataswich myData;

///////////funstion ///////////////
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);

//////////////////////////////////

//callback function that will be executed when data is received

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("x1: ");
  Serial.println(myData.s1);
  Serial.print("x2: ");
  Serial.println(myData.s2);
  Serial.print("x3: ");
  Serial.println(myData.s3);
  Serial.println();
  if(myData.s1 == 1){
     digitalWrite(LED_S1,1);
     Serial.println("A");
  }else if(myData.s2 == 1){ 
    digitalWrite(LED_S2,1);
    Serial.println("B");
  }else if(myData.s3 == 1){
    digitalWrite(LED_S3, 1);
    Serial.println("C");
  }else {
    digitalWrite(LED_S1, 0);
    digitalWrite(LED_S2, 0);
    digitalWrite(LED_S3, 0);
    Serial.println("E");
  }
}
 
void setup() {
  //Initialize Serial Monitor
  Serial.begin(115200);
  
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  pinMode(LED_S1, OUTPUT);
  pinMode(LED_S2, OUTPUT);
  pinMode(LED_S3, OUTPUT);


  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {
  
}