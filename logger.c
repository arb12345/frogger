#include <msp430.h>
#include <stdint.h>
#include "backchannel_uart.h"
#include "logger.h"

static uint16_t time_capture_value = 0;

void logger_init()
{
	backchannel_uart_init();
}

void logger_time_capture_start()
{
	TB0EX0 = TBIDEX_3;
	TB0CCTL0 = CAP | SCS | CCIS1 | CM_3;
	TB0CTL = TBSSEL__ACLK | MC__CONTINUOUS | ID__8 | TBCLR;
}

void logger_time_capture_stop()
{
	TB0CCTL0 ^= CCIS0;
	while((TB0CCTL0 & CCIFG) == 0);
	time_capture_value = TB0CCR0;
	TB0CCTL0 = 0;
	TB0CTL = MC__STOP;
}

void logger_time_capture_print_info()
{
	uint8_t str[6];
	uint8_t size;
	if(time_capture_value < 10)
	{
		str[0] = time_capture_value + '0';
		size = 1;
	} else if(time_capture_value < 100)
	{
		str[0] = (time_capture_value/10) + '0';
		str[1] = (time_capture_value%10) + '0';
		size = 2;
	} else if(time_capture_value < 1000)
	{
		str[0] = (time_capture_value/100) + '0';
		time_capture_value = time_capture_value % 100;
		str[1] = (time_capture_value/10) + '0';
		str[2] = (time_capture_value%10) + '0';
		size = 3;
	} else if(time_capture_value < 10000)
	{
		str[0] = (time_capture_value/1000) + '0';
		time_capture_value = time_capture_value % 1000;
		str[1] = (time_capture_value/100) + '0';
		time_capture_value = time_capture_value % 100;
		str[2] = (time_capture_value/10) + '0';
		str[3] = (time_capture_value%10) + '0';
		size = 4;
	} else
	{
		str[0] = (time_capture_value/10000) + '0';
		time_capture_value = time_capture_value % 10000;
		str[1] = (time_capture_value/1000) + '0';
		time_capture_value = time_capture_value % 1000;
		str[2] = (time_capture_value/100) + '0';
		time_capture_value = time_capture_value % 100;
		str[3] = (time_capture_value/10) + '0';
		str[4] = (time_capture_value%10) + '0';
		size = 5;
	}
	str[size] = '\n';
	size++;
	backchannel_uart_write(str, size);
}
