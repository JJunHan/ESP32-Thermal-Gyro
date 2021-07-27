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
#include <melody_player.h>
#include <melody_factory.h>

#define BUZZER 19
#define SW1 12 // Can be used to toggle
#define SW3 13


int val = 0;

// Buzzer implementation 
MelodyPlayer player1(19);
String notes1[] = { "C4", "G3", "G3", "A3", "G3", "SILENCE", "B3", "C4" };
Melody melody1 = MelodyFactory.load("Nice Melody", 500, notes1, 8);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(BUZZER,OUTPUT);
  ledcSetup(0, 2000, 8); //8 bit resolution max 255
  ledcAttachPin(BUZZER, 0);
  pinMode(SW1, INPUT);
  
 
  Serial.print("Playing... ");
  //player1.playAsync(melody1);
}

void loop() {
  
  //ledcWriteTone(0, 2000);
  //ledcWrite(0, 100);
  //delay(1000);

  // Reading input from SW1
  val = digitalRead(12);
  Serial.println(val);
  
  if(!val){
    //digitalWrite(BUZZER,HIGH); // BUZZER RINGS
    player1.playAsync(melody1);
  }
  else{
    digitalWrite(BUZZER,LOW);
  }
  
  delay(500);
  


}
