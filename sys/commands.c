//------------------------------------------------------------------------------
//
// Module : CMD_
//
// Description : Commands parser Module Core
//
// F.C.
// 2014.08.22
//
//------------------------------------------------------------------------------
#define CMD_M

//------------------------------------------------------------------------------
//                                                                      Includes
//------------------------------------------------------------------------------
#include "commands_table.h"
#include "commands.h"
#include "comm.h"
#include <string.h>
//------------------------------------------------------------------------------
//                                                               Defines & Types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                                                                     Variables
//------------------------------------------------------------------------------
// Note : OBLIGATOIREMENT static
extern const S_CommandTableElt CommandTable[]; //reference to the lookup table.

//------------------------------------------------------------------------------
//                                             Prototypes des fonctions internes
//------------------------------------------------------------------------------
// Note : OBLIGATOIREMENT static

//------------------------------------------------------------------------------
//                                                           Fonctions exportees
//------------------------------------------------------------------------------

/* Search for the callback linked to a given command ID
 * and execute it. Returns Ret_OK if the command exists,
 * Ret_Error otherwise.
 */
Ret_t CMD_Execute(S_COMMAND* cmd)
{
	pCommand fPtr = NULL;
	UINT8 i;
	if (cmd->eCommandStatus != COMMAND_CHECKED)
		return RET_ERROR;

	/* look for the command in the table */
	for (i=0; i < CommandsTableSize && !fPtr; ++i)
	{
		if (CommandTable[i].cmd == cmd->cmd)
		{
			fPtr = CommandTable[i].fPtr;
			(*fPtr)(cmd);
		}
	}
	return (fPtr? RET_OK : RET_ERROR);
}

/* Test command that will togle the pin13 led
 * on the arduino UNO.
 */
void CMD_TestCommand(S_COMMAND* cmd)
{
	S_COMMAND reply;

	DDRB = (1<<DDB5);
	PORTB ^= (1<<PB5);

	reply.cmd = CMD_TEST;
	reply.payload = 4;
	memcpy(reply.data, (UINT8 [4]) {1, 2, 4, 8}, 4 * sizeof(UINT8));
	COMM_SendCommand(&reply);
}
//------------------------------------------------------------------------------
//                                                           Fonctions internes
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
