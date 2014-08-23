/*
 * test_uart.c
 *
 *  Created on: 16 août 2014
 *      Author: fred
 */

#include <util/delay.h>

#include "common.h"
#include "uart.h"

//simple echo server on uart.
void test_uart(void)
{
	UINT8 data = NULL;

	UART_Init(CFG_UART_BAUDRATE);
	UART_Flush_rx();

	UART_puts("Ready!\r\n");
	while (1)
	{
		UART_Read_u8(&data);
		UART_Write_u8(data);
	}
}
