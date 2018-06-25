#include "backchannel_uart.h"
#include "fifo.h"

FIFO_CREATE(uart_tx,32,uint8_t);
FIFO_CREATE(uart_rx,32,uint8_t);

void backchannel_uart_init()
{
	P4SEL |= BIT4 | BIT5;
	UCA1CTL1 |= UCSWRST;
	UCA1CTL1 |= UCSSEL__ACLK;
	UCA1BRW = 3;
	UCA1MCTL = UCBRS_3;
	UCA1CTL1 &= ~UCSWRST;
	uart_tx_fifo_init();
	uart_rx_fifo_init();
	UCA1IE |= UCRXIE;
}

void backchannel_uart_write(uint8_t *buffer, uint16_t length)
{
	while(length)
	{
		while(uart_tx_fifo_insert(*buffer) == 0);
		UCA1IE |= UCTXIE;
		buffer++;
		length--;
	}
}

#pragma vector=USCI_A1_VECTOR
__interrupt void uart_a1_isr()
{
	uint8_t data;
	switch(__even_in_range(UCA1IV, 0x4))
	{
		case 0x00:
			break;
		case 0x02:
			uart_rx_fifo_insert(UCA1RXBUF);
			break;
		case 0x04:
			if(uart_tx_fifo_remove(&data) == 0)
			{
				UCA1IE &= ~UCTXIE;
				UCA1IFG |= UCTXIFG;
			} else
			{
				UCA1TXBUF = data;
			}
			break;
		default:
			break;
	}
}

uint16_t backchannel_uart_read(uint8_t *buffer, uint16_t length)
{
	uint16_t i = 0;
	for (i = 0; i < length; ++i) {
		if(uart_rx_fifo_remove(&buffer[i]) == 0)
		{
			break;
		}
	}
	return i;
}
