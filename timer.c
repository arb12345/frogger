#include "timer.h"
#include <msp430.h>

#define TIMER_A1_T10Hz 	3277
#define TIMER_A1_T12Hz	2731
#define TIMER_A1_T15Hz	2185
#define TIMER_A1_T20Hz	1638

void timer_a0_delay_us(uint16_t delay_us)
{
	TA0CCR0 = delay_us-1;
	TA0EX0 = TAIDEX_5;
	TA0CTL = TASSEL__SMCLK | MC__UP | ID__4 | TACLR;
	while((TA0CCTL0 & CCIFG) == 0);
	TA0CCTL0 &= ~CCIFG;
	TA0CTL = MC__STOP;
	TA0EX0 = 0;
}

void timer_a1_init_10Hz()
{
	TA1CCR0 = TIMER_A1_T10Hz-1;
	TA1CCTL0 = CCIE;
	TA1CTL = TASSEL__ACLK | MC__UP | TACLR;
}

void timer_a1_init_12Hz()
{
	TA1CCR0 = TIMER_A1_T12Hz-1;
	TA1CCTL0 = CCIE;
	TA1CTL = TASSEL__ACLK | MC__UP | TACLR;
}

void timer_a1_init_15Hz()
{
	TA1CCR0 = TIMER_A1_T15Hz-1;
	TA1CCTL0 = CCIE;
	TA1CTL = TASSEL__ACLK | MC__UP | TACLR;
}

void timer_a1_init_20Hz()
{
	TA1CCR0 = TIMER_A1_T20Hz-1;
	TA1CCTL0 = CCIE;
	TA1CTL = TASSEL__ACLK | MC__UP | TACLR;
}

void timer_a1_stop()
{
	TA1CCTL0 &= ~CCIE;
	TA1CTL = MC__STOP;
}
