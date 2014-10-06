//------------------------------------------------------------------------------
//
// Module : CMD_
//
// Description : Commands for exchanging data.
//
// F.C.
// 2014.10.06
//
//------------------------------------------------------------------------------
#define APP_CMD_M

//------------------------------------------------------------------------------
//                                                                      Includes
//------------------------------------------------------------------------------
#include <avr/io.h>
#include <string.h> //memcpy

#include "comm.h"

#include "CMD.h"

//------------------------------------------------------------------------------
//                                                               Defines & Types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                                                                     Variables
//------------------------------------------------------------------------------
// Note : OBLIGATOIREMENT static

//------------------------------------------------------------------------------
//                                             Prototypes des fonctions internes
//------------------------------------------------------------------------------
// Note : OBLIGATOIREMENT static

//------------------------------------------------------------------------------
//                                                           Fonctions exportees
//------------------------------------------------------------------------------
/* Test command that will togle the pin13 led
 * on the arduino UNO.
 */
void CMD_RX_TestCommand(S_COMMAND* cmd)
{
	S_COMMAND reply;

	DDRB = (1<<DDB5);
	PORTB ^= (1<<PB5);

	reply.cmd = CMD_TEST;
	reply.payload = 4;
	memcpy(reply.data, (UINT8 [4]) {1, 2, 4, 8}, 4 * sizeof(UINT8));
	COMM_SendCommand(&reply);
}

//Test command. Send a counter value, incremented each time
void CMD_TX_SendDummyCommand(void)
{
	static UINT16 cnt = 0;
	S_COMMAND cmd;

	cmd.cmd = 18;
	cmd.payload = 2;
	cmd.data[0] = cnt >> 8;
	cmd.data[1] = cnt & 0xFF;

	COMM_SendCommand(&cmd);
	cnt++;
}

//------------------------------------------------------------------------------
//                                                           Fonctions internes
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
