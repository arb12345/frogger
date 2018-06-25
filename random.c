#include "random.h"

#define RANDOM_M 			49381
#define RANDOM_I 			8643

static uint16_t random_state;
void random_init()
{
	uint16_t i, j;
	random_state = 0;

	uint16_t UCSCTL4_old = UCSCTL4;
	uint16_t TA0CTL_old = TA0CTL;
	uint16_t TA0CCTL2_old = TA0CCTL2;

	TA0CTL = 0x00;
	UCSCTL4 = SELS__DCOCLK | SELA__VLOCLK;
	TA0CCTL2 = CAP | SCS | CM_1 | CCIS_1;
	TA0CTL = TASSEL__SMCLK | MC__CONTINUOUS;

	for (i = 0; i < 16; ++i) {
		uint16_t ones = 0;
		for (j = 0; j < 5; ++j) {
			while(!(CCIFG & TA0CCTL2));
			TA0CCTL2 &= ~CCIFG;

			if(1 & TA0CCR2)
			{
				ones++;
			}
		}
		random_state >>= 1;

		if(ones >= 3)
		{
			random_state |= 0x8000;
		}
	}

	UCSCTL4 = UCSCTL4_old;
	TA0CTL = TA0CTL_old;
	TA0CCTL2 = TA0CCTL2_old;
}

uint16_t random()
{
	random_state = RANDOM_M * random_state + RANDOM_I;
	return random_state;
}
