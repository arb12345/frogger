#ifndef TIMER_H_
#define TIMER_H_
#include <stdint.h>

// Assumes SMCLK @ 25.000.000Hz
void timer_a0_delay_us(uint16_t delay_us);

// Assumes ACLK @ 32.768Hz
void timer_a1_init_10Hz();
void timer_a1_init_12Hz();
void timer_a1_init_15Hz();
void timer_a1_init_20Hz();
void timer_a1_stop();

#endif /* TIMER_H_ */
