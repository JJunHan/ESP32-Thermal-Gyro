#ifndef LED1642
#define LED1642

#include <Arduino.h>

#define NUM_LED1642GW_ICs (1)
#define PIN_CLK (27)
#define SET_CLK_H() (digitalWrite(PIN_CLK, HIGH))
#define SET_CLK_L() (digitalWrite(PIN_CLK, LOW))
#define PIN_SDI (26)
#define SET_SDI_H() (digitalWrite(PIN_SDI, HIGH))
#define SET_SDI_L() (digitalWrite(PIN_SDI, LOW))
#define PIN_LE (14)
#define SET_LE_H() (digitalWrite(PIN_LE, HIGH))
#define SET_LE_L() (digitalWrite(PIN_LE, LOW))

void led1642gw_turn_all_on(void);
void led1642gw_turn_all_off(void);
void led1642gw_init(void);
void led1642gw_clear(void);
void led1642gw_flush_config();
void led1642gw_flush(void);
void led1642gw_set_ALL();
void led1642gw_set_channel(uint8_t channel, uint16_t value);

#endif