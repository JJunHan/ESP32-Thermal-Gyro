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

#define BUZZER 19
#define SW1 12 // Can be used to toggle
#define SW3 13


void activateBuzzer(boolean state);
int val = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  ledcSetup(3, 8000, 12);
  ledcAttachPin(BUZZER, 3);
  pinMode(SW1, INPUT);

  pinMode(BUZZER, OUTPUT);
}

void loop() {
  
  //activateBuzzer(true);
  
  // Reading input from SW1
  val = digitalRead(12);
  Serial.println(val);
  if(!val){
    digitalWrite(BUZZER,HIGH); // BUZZER RINGS
  }
  
  delay(500);

}

void activateBuzzer(boolean state){
  // If the state is true, activate the buzzer with the given frequency.
  if(state) ledcWriteTone(3, 500);
}