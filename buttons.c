#include "buttons.h"

#define BUTTON_UP_DIR		P4DIR
#define BUTTON_UP_REN		P4REN
#define BUTTON_UP_OUT		P4OUT
#define BUTTON_UP_IN		P4IN
#define BUTTON_UP_PIN		BIT0

#define BUTTON_DOWN_DIR		P3DIR
#define BUTTON_DOWN_REN		P3REN
#define BUTTON_DOWN_OUT		P3OUT
#define BUTTON_DOWN_IN		P3IN
#define BUTTON_DOWN_PIN		BIT7

#define BUTTON_MUTE_DIR		P2DIR
#define BUTTON_MUTE_REN		P2REN
#define BUTTON_MUTE_OUT		P2OUT
#define BUTTON_MUTE_IN		P2IN
#define BUTTON_MUTE_PIN		BIT1

uint8_t button_up = 0;
uint8_t button_down = 0;
uint8_t button_mute_state = 0;

void buttons_init()
{
	BUTTON_UP_DIR &= ~BUTTON_UP_PIN;
	BUTTON_UP_REN |= BUTTON_UP_PIN;
	BUTTON_UP_OUT |= BUTTON_UP_PIN;

	BUTTON_DOWN_DIR &= ~BUTTON_DOWN_PIN;
	BUTTON_DOWN_REN |= BUTTON_DOWN_PIN;
	BUTTON_DOWN_OUT |= BUTTON_DOWN_PIN;

	BUTTON_MUTE_DIR &= ~BUTTON_MUTE_PIN;
	BUTTON_MUTE_REN |= BUTTON_MUTE_PIN;
	BUTTON_MUTE_OUT |= BUTTON_MUTE_PIN;
}


void buttons_read()
{
	button_up = BIT0 & (~(BUTTON_UP_IN & BUTTON_UP_PIN));
	button_down = BIT0 & (~((BUTTON_DOWN_IN & BUTTON_DOWN_PIN) >> 7));
}

uint8_t button_mute_pressed()
{
	uint8_t button_mute_next_state = BIT0 & (~((BUTTON_MUTE_IN & BUTTON_MUTE_PIN) >> 1));
	uint8_t button_pressed = button_mute_next_state && (button_mute_state == 0);
	button_mute_state = button_mute_next_state;
	return button_pressed;
}
