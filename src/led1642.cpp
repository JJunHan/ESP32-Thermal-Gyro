#include "led1642.h"
#include <string.h>
#include <stdint.h>

#define NUM_LED1642GW_ICs (1)
#define NUM_LED1642GW_CHANNELS (16)
#define NUM_LED_CHANNELS (NUM_LED1642GW_CHANNELS*NUM_LED1642GW_ICs)


static uint16_t ledbuffer[16] = {
                                0xffff, 0xffff, 0xffff, 0xffff, 
                                0xffff, 0xffff, 0xffff, 0xffff, 
                                0x0000, 0x0000, 0x0000, 0x0000, //THIS ARE CONTROLS FOR 7 SEG
                                0xffff, 0x0000, 0xffff, 0x0000 //THIS ARE LEDS
                                };
static uint16_t config_reg[NUM_LED1642GW_ICs];


static void write_data(uint16_t data, uint8_t le_clocks)
{
  uint16_t mask = 0x8000;
  int8_t bit;
  SET_LE_L();
  for (bit=15; bit>=le_clocks; bit--) {
	SET_CLK_L(); 
    if(data&mask) { SET_SDI_H(); }
    else { SET_SDI_L(); }
	SET_CLK_H();
    mask >>= 1;
  }
  
  SET_LE_H();

  for (/*noting to initialize*/; bit>=0; bit--) {
	  SET_CLK_L();
    if(data&mask) { SET_SDI_H(); }
    else { SET_SDI_L(); }
	SET_CLK_H();
    mask >>= 1;
 }
  
  // set all pins to low after transmission
  SET_CLK_L();
  SET_LE_L();
  SET_SDI_L();
  
}

static void write_data_latch(uint16_t data)
{
  write_data(data, 3);
}

static void write_global_latch(uint16_t data)
{
  write_data(data, 5);
}

static void write_no_command(uint16_t data)
{
  write_data(data, 0);
}

void led1642gw_turn_all_on(void)
{
  
  write_data(0xffff, 2);
  delayMicroseconds(10);
  write_data(0xffff, 2);
  delayMicroseconds(10);
  write_data(0xffff, 2);
  delayMicroseconds(10);
}


void led1642gw_turn_all_off(void)
{
  
  write_data(0x0000, 2);
  delayMicroseconds(10);
  write_data(0x0000, 2);
  delayMicroseconds(10);
  write_data(0x0000, 2);
  delayMicroseconds(10);
}

void led1642gw_init(void)
{
  SET_CLK_L();
  SET_SDI_L();
  SET_LE_L();
  pinMode(PIN_CLK, OUTPUT);
  pinMode(PIN_SDI, OUTPUT);
  pinMode(PIN_LE, OUTPUT);
  //memset(ledbuffer, 0x00, sizeof(ledbuffer));
  memset(config_reg, 0x00, sizeof(config_reg));
  //led1642gw_flush_config();
  //led1642gw_flush();
}

void led1642gw_clear(void)
{
  memset(ledbuffer, 0x00, sizeof(ledbuffer));
  led1642gw_flush();
}

void led1642gw_flush_config()
{
  write_data(config_reg[0], 7);
}

void led1642gw_flush(void)
{
  uint8_t channel;
  for (channel=0; channel<NUM_LED1642GW_CHANNELS-1; channel++) {
    write_data_latch(ledbuffer[channel]);

  }
    write_global_latch(ledbuffer[(NUM_LED1642GW_CHANNELS)-1]);
}

void led1642gw_set_channel(uint8_t channel, uint16_t value)
{
  if (channel < NUM_LED_CHANNELS) {
    ledbuffer[channel] = value;
    //Serial.println(value);
    
  }
}

void led1642gw_set_ALL(){
  Serial.println("empty");
}
