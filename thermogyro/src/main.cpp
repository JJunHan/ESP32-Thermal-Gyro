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

void activateBuzzer(boolean state);
int val = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  ledcSetup(3, 8000, 12);
  ledcAttachPin(BUZZER, 3);
  pinMode(12, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  activateBuzzer(true);
  val = digitalRead(12);
  Serial.println(val);
  delay(500);
}

void activateBuzzer(boolean state){
  // If the state is true, activate the buzzer with the given frequency.
  if(state) ledcWriteTone(3, 500);
}