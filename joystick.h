#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <msp430.h>
#include "timer.h"
#include <stdint.h>

#define JOYSTICK_THRESHOLD_LEFT		819
#define JOYSTICK_THRESHOLD_RIGHT	3276

typedef enum
{
	JOYSTICK_DIR_MIDDLE = 0,
	JOYSTICK_DIR_LEFT,
	JOYSTICK_DIR_RIGHT
} joystick_dir_t;

extern volatile joystick_dir_t joystick_direction;
void joystick_init();
void joystick_read();

#endif /* JOYSTICK_H_ */
