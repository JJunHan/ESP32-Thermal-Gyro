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

// Server Variables
const char* _ssid = "SINGTEL-BE9C (2.4G)";
const char* _password = "oopheishoh";
AsyncWebServer server(80);

// GPIO Variables
int SW1_value = 0;

// Gyro Variables
MPU6050 accelgyro;
//float temperature;
int16_t ax, ay, az;
int16_t gx, gy, gz;

// Gyroscope sensor deviation
float gyroXerror = 0.07;
float gyroYerror = 0.03;
float gyroZerror = 0.01;

// Temperature & Humidity Variables
ClosedCube_HDC1080 sensor;
float temperature = 0, humidity = 0;

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

void setup() {
  Serial.begin(115200);
  while(!Serial);
  //initWIFI();
  //initSPIFFS();
  initMPU();
  //initTEMP();
  //initBUZZER();

  // List all files in the flash system
  //listDir(SPIFFS,"/",3);

  // Declaration of GPIO Pins
  pinMode(BUZZER_PIN,OUTPUT);
  pinMode(SW1_PIN, INPUT);
  pinMode(SW3_PIN, INPUT);

  // Set up LED Driver Channel


  // Declaration of LED driver config
  /*
  led1642gw_init();
  led1642gw_turn_all_on(); // turn on all switches 
  led1642gw_flush(); //push the ledbuffer into the registers accordingly
  */
  
}

void loop() {
  
  //Serial.print("Temperature (C): "); Serial.print(sensor.readTemperature());
  //Serial.print("\t\tHumidity (%): "); Serial.println(sensor.readHumidity());
  //delay(1000);

/*
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
  delay(500);
*/

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
