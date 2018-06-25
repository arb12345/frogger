#ifndef BACKCHANNEL_UART_H_
#define BACKCHANNEL_UART_H_

#include <msp430.h>
#include <stdint.h>

void backchannel_uart_init();
void backchannel_uart_write(uint8_t *buffer, uint16_t length);
uint16_t backchannel_uart_read(uint8_t *buffer, uint16_t length);

#endif /* BACKCHANNEL_UART_H_ */
