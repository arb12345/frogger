#ifndef RANDOM_H_
#define RANDOM_H_

#include <msp430.h>
#include <stdint.h>

void random_init();
uint16_t random();

#endif /* RANDOM_H_ */
