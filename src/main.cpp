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

#define BUZZER_PIN 19
#define BUZZER_CHANNEL 5
#define SW1_PIN 12 // Can be used to toggle
#define SW3_PIN 13 
#define LED_LE 14
#define LED_CLK 27
#define LED_SDI 26

int SW1_value = 0;

void setup() {
  Serial.begin(115200);
  
  // Declaration of GPIO Pins
  pinMode(BUZZER_PIN,OUTPUT);
  pinMode(SW1_PIN, INPUT);
  pinMode(SW3_PIN, INPUT);

  // Set up Buzzer Channel
  ledcSetup(BUZZER_CHANNEL, 2000, 8); //8 bit resolution max 255
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);

  // Set up LED Driver Channel

  ledcSetup(1, 0, 8); //8 bit resolution max 255
  ledcAttachPin(LED_CLK, 1);

  // Set up Library variables
  cute.init(BUZZER_PIN);
  
  // Declaration of LED driver config
  led1642gw_init();
  led1642gw_turn_all_on(); // turn on all switches 
  led1642gw_flush(); //push the ledbuffer into the registers accordingly
  
  
}

void loop() {
  
  // Reading input from SW1
  SW1_value = digitalRead(12);
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
  
}
