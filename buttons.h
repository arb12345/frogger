#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <msp430.h>
#include <stdint.h>

extern uint8_t button_up;
extern uint8_t button_down;
void buttons_init();
void buttons_read();
uint8_t button_mute_pressed();

#endif /* BUTTONS_H_ */
