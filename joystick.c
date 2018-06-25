#include "joystick.h"

// A5  (P6.5)
#define ADC12_A_A5SEL		P6SEL
#define ADC12_A_A5_PIN		BIT5

volatile joystick_dir_t joystick_direction=JOYSTICK_DIR_MIDDLE;

inline void adc12_a_init()
{
	ADC12_A_A5SEL |= ADC12_A_A5_PIN;
	ADC12CTL0 &= ~ADC12ENC; 	//Disabling conversion to configure ADC12
	ADC12CTL0 = ADC12SHT0_2 | ADC12ON;
								//sampling-and-hold time =>  16 ADC12CLK cycles + 13 cycles =  29 cycles
								//ADC12_A on
	ADC12CTL1 = ADC12SHP;
	ADC12CTL2 = ADC12RES_2 | ADC12TCOFF;
							//12 bit of resolution
							//Temperature sensor off
    ADC12MCTL0 = ADC12SREF_0 | ADC12INCH_5;
    						//V(R+) = AVCC and V(R-) = AVSS
    						//A5 channel selected
    //ADC12IE = ADC12IE0;          //ADC12 interrupt enable in ADC12MEM0
    ADC12CTL0 |= ADC12ENC;
}

inline void adc12_a_start_conversion()
{
	ADC12CTL0 |= ADC12SC;
}

void joystick_init()
{
	adc12_a_init();
}

void joystick_read()
{
	adc12_a_start_conversion();
	while((ADC12IFG & ADC12IFG0) == 0);
	uint16_t value = ADC12MEM0;
	if(value < JOYSTICK_THRESHOLD_LEFT)
	{
		joystick_direction = JOYSTICK_DIR_LEFT;
	} else if(value > JOYSTICK_THRESHOLD_RIGHT)
	{
		joystick_direction = JOYSTICK_DIR_RIGHT;
	} else
	{
		joystick_direction = JOYSTICK_DIR_MIDDLE;
	}
}
