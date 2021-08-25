/**
 * @file main.cpp
 * @author Junhan 
 * @brief Just a fun mini project that i designed!
 * @version 0.1
 * @date 2021-07-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <Arduino.h>
#include <CuteBuzzerSounds.h>
#include "led1642.h"
#include "SPIFFS.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "files.h"
#include "ClosedCube_HDC1080.h"
#include <ArduinoJson.h>

#define ADDR_TEMP 0x40
#define BUZZER_PIN 19
#define BUZZER_CHANNEL 5
#define SW1_PIN 12 // Can be used to toggle
#define SW3_PIN 13 
#define LED_LE 14 
#define LED_CLK 27
#define LED_SDI 26
#define SDA_1 32
#define SCL_1 33
#define SDA_2 23
#define SCL_2 22

//#define DISPLAY_ALL //uncomment this to show all readings via serial monitor


// Server Variables
const char* _ssid = "SINGTEL-BE9C (2.4G)";
const char* _password = "x";
AsyncWebServer server(80);
String processors(const String& var);
StaticJsonDocument<200> doc;
AsyncEventSource events("/events");


// GPIO Variables
int SW1_value = 0;

// Gyro Variables
MPU6050 accelgyro;
//float temperature;
int16_t ax, ay, az;
int16_t gx, gy, gz;
char gyro[128];
char temp[128];
char humi[128];

// Temperature & Humidity Variables
ClosedCube_HDC1080 sensor;
float temperature = 0, humidity = 0;

// Timer variables
unsigned long lastTime = 0;  
unsigned long lastTimeTemperature = 0;
unsigned long lastTimeAcc = 0;
unsigned long gyroDelay = 500;
unsigned long temperatureDelay = 1000;
unsigned long accelerometerDelay = 500;


void initTEMP(){
  sensor.begin(ADDR_TEMP);
}

void initMPU(){

  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin(SDA_2,SCL_2,100000);
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
  #endif
  
  Serial.println("Successfully Connected");

  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

}

void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("Error mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

void initWIFI(){
  WiFi.begin(_ssid, _password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
}

void initBUZZER(){
  // Set up Buzzer Channel
  ledcSetup(BUZZER_CHANNEL, 2000, 8); //8 bit resolution max 255
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);

  // Set up Library variables
  cute.init(BUZZER_PIN);
}

String processors(const String& var){

  snprintf(gyro, sizeof(gyro), "%d | %d | %d | %d | %d | %d ", ax,ay,az,gx,gy,gz);
  snprintf(temp, sizeof(temp), "%f", sensor.readTemperature());
  snprintf(humi, sizeof(humi), "%f", sensor.readHumidity());
  
  if(var == "GyroPlaceholder"){
    return gyro;
  }
  else if(var == "TempPlaceholder"){
    return temp;
  }
  else if(var == "HumidityPlaceholder"){
    return humi;
  }
  return String();
}

String getAccReadings(){
  String holder;
  doc["accX"] = String(ax);
  doc["accY"] = String(ay);
  doc["accZ"] = String(az);
  serializeJson(doc, holder);
  //Serial.println(holder);
  doc.clear();
  return String(holder);
  
}

String getGyroReadings(){
  String holder;
  doc["gyroX"] = String(gx);
  doc["gyroY"] = String(gy);
  doc["gyroZ"] = String(gz);
  serializeJson(doc, holder);
  //Serial.println(holder);
  doc.clear();
  return String(holder);
}

String getTemp_Humi(){
  String holder;
  doc["temp"] = String(sensor.readTemperature());
  doc["humi"] = String(sensor.readHumidity());
  serializeJson(doc, holder);
  //Serial.println(holder);
  doc.clear();
  return String(holder);
}


void setup() {
  Serial.begin(115200);
  while(!Serial);
  initWIFI();
  initSPIFFS();
  initMPU();
  initTEMP();
  //initBUZZER();

/*
  doc["sensor"] = "gps";
  doc["time"] = 1351824120;
  JsonArray data = doc.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);

  serializeJson(doc, Serial);
  Serial.println();
  serializeJsonPretty(doc, Serial);
*/

  // List all files in the flash system
  //listDir(SPIFFS,"/",3);

  // Declaration of GPIO Pins
  pinMode(BUZZER_PIN,OUTPUT);
  pinMode(SW1_PIN, INPUT);
  pinMode(SW3_PIN, INPUT);

  // Declaration of LED driver config
  led1642gw_init();
  led1642gw_flush(); //push the ledbuffer into the registers accordingly
  led1642gw_turn_all_on(); // turn on all switches 
  
  // Route for root web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html", false, processors);
  });

  // Async sends number of detected targets to HTTP Server
  server.on("/gyro", HTTP_GET, [](AsyncWebServerRequest *request){
    snprintf(gyro, sizeof(gyro), "%d | %d | %d | %d | %d | %d ", ax,ay,az,gx,gy,gz);
    request->send(200, "text/plain", gyro);
  });
  server.on("/temp", HTTP_GET, [](AsyncWebServerRequest *request){
    snprintf(temp, sizeof(temp), "%f", sensor.readTemperature());
    request->send(200, "text/plain", temp);
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    snprintf(humi, sizeof(humi), "%f", sensor.readHumidity());
    request->send(200, "text/plain", humi);
  });


  /*
    // This to attach images
  server.on("/owl", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/owl.png", "image/png");
  });
  */

  // This to send chart.min.js via spiff
  
  server.on("/three.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/three.min.js", "text/javascript");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/script.js", "text/javascript");
  });
  
  server.serveStatic("/", SPIFFS, "/");
  
  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    gx=0;
    gy=0;
    gz=0;
    request->send(200, "text/plain", "OK");
  });

  // On error requests
  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404, "text/plain", "The content you are looking for was not found.");
  });

  // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  // Start server
  server.begin();

}

void loop() {

  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  if ((millis() - lastTime) > gyroDelay) {
    // Send Events to the Web Server with the Sensor Readings
    events.send(getGyroReadings().c_str(),"gyro_readings",millis());
    lastTime = millis();
  }
  if ((millis() - lastTimeAcc) > accelerometerDelay) {
    // Send Events to the Web Server with the Sensor Readings
    events.send(getAccReadings().c_str(),"accelerometer_readings",millis());
    lastTimeAcc = millis();
  }
  if ((millis() - lastTimeTemperature) > temperatureDelay) {
    // Send Events to the Web Server with the Sensor Readings
    events.send(getTemp_Humi().c_str(),"temperature_reading",millis());
    lastTimeTemperature = millis();
  }



  #ifdef DISPLAY_ALL
  // Humidity and Temperature readings
  Serial.print("Temperature (C): "); Serial.print(sensor.readTemperature());
  Serial.print("\t\tHumidity (%): "); Serial.println(sensor.readHumidity());

  // Gyro and Acc readings
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  Serial.print("a/g:\t");
  Serial.print(ax); Serial.print("\t");
  Serial.print(ay); Serial.print("\t");
  Serial.print(az); Serial.print("\t");
  Serial.print(gx); Serial.print("\t");
  Serial.print(gy); Serial.print("\t");
  Serial.println(gz);
  Serial.print("Temperature of gyro: ");
  Serial.println((accelgyro.getTemperature() + 521)/340 + 35);
  delay(1000);

  #else
    
  #endif


  // Reading input from SW1
  /*SW1_value = digitalRead(12);
  //Serial.println(SW1_value);
  
  if(!SW1_value){ // Upon button press
    
    // Using a Library
    //cute.play(S_CONNECTION);
    
    // Manually play the Ding Dong Chime
    ledcWriteTone(BUZZER_CHANNEL, 698);
    delay(500);     // milliseconds
    ledcWrite(BUZZER_CHANNEL, 0); // no tone as no duty cycle
    delay(50);
    ledcWriteTone(BUZZER_CHANNEL, 523);     
    delay(600);
    ledcWrite(BUZZER_CHANNEL, 0);
    
  }
  else{
    digitalWrite(BUZZER_PIN,LOW);
  }
  */
}
