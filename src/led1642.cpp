#include "led1642.h"
#include <string.h>
#include <stdint.h>

#define NUM_LED1642GW_CHANNELS (16)
#define NUM_LED_CHANNELS (NUM_LED1642GW_CHANNELS*NUM_LED1642GW_ICs)


static uint16_t ledbuffer[16] = {
                                0x0000, 0x0000, 0x0000, 0x0000, 
                                0xffff, 0xffff, 0xffff, 0xffff,
                                0x0000, 0x0000, 0x0000, 0x0000, 
                                0x0000, 0x0000, 0x0000, 0x0000
                                };
static uint16_t config_reg[NUM_LED1642GW_ICs];


static void write_data(uint16_t data, uint8_t le_clocks)
{
  uint16_t mask = 0x8000;
  int8_t bit;

  SET_CLK_L(); 
  SET_LE_L();
  for (bit=15; bit>=le_clocks; bit--) {
    if(data&mask) { SET_SDI_H(); 
    }
    else{
      SET_SDI_L();
    }
    
	  SET_CLK_H();
    mask >>= 1;
	  SET_CLK_L(); 
  }
  
  SET_LE_H();
  for (/*noting to initialize*/; bit>=0; bit--) {
    //Serial.println(bit);
    if(data&mask) { SET_SDI_H(); }
    else{
      SET_SDI_L();
    }
	SET_CLK_H();
    mask >>= 1;
	SET_CLK_L();
 }
  
  // set all pins to low after transmission
  //SET_CLK_L();
  SET_LE_L();
  //SET_SDI_L();
  
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
  write_data(0xffff, 1); // write into write switch to turn on all
}

void led1642gw_turn_all_off(void)
{
  write_data(0x0000, 1); // write into write switch to turn off all
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
}

void led1642gw_flush_config()
{
  write_data(config_reg[0], 7);
}

void led1642gw_flush(void)
{
  uint8_t channel;
  uint8_t ic;

  // for each of the first 15 channels, do the following:
  for (channel=0; channel<NUM_LED1642GW_CHANNELS; channel++) {
    // shift data throught the first n-1 ICs with write_no_command
    for (ic=0; ic<(NUM_LED1642GW_ICs-1); ic++) {
      write_no_command(ledbuffer[channel+(NUM_LED1642GW_CHANNELS*ic)]);
      Serial.println("imin");
    }
    
    // then, when the brightness data has propagated through the
    // shift registers, write all data into the DATA LATCH of
    // all of the ICs.
    // for the 16th channel, we don't write to the DATA LATCH, but
    // to the CLOBAL data latch.
    // once more, we do the trick with write_no_command, to 
    // shift data through all the ICs
    if (channel < NUM_LED1642GW_CHANNELS-1) {
      write_data_latch(ledbuffer[channel]);
      Serial.print(channel);
      Serial.println(ledbuffer[channel]);
    }
    else {
      write_global_latch(ledbuffer[channel]);
    }
  }
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
