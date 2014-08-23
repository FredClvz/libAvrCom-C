//------------------------------------------------------------------------------
//
// Module : CMD_
//
// Description : Commands table: link a given command to a program's function pointer
//
// F.C.
// 2014.08.22
//
//------------------------------------------------------------------------------

// Multi-inclusion protection

#ifndef COMMANDS_TABLE_H_
#define COMMANDS_TABLE_H_

#include "common.h"

//------------------------------------------------------------------------------
//                                                 Command definitions inclusion
//------------------------------------------------------------------------------
#include "commands.h"

//------------------------------------------------------------------------------

typedef void (*pCommand)(S_COMMAND*);

typedef struct
{
	UINT8 cmd;
	pCommand fPtr;
}S_CommandTableElt;

/* A command pointed to should have a definition similar to the following:
 * void function(S_COMMAND$ cmd)
 */
const S_CommandTableElt CommandTable[] =
{
	{CMD_TEST, &CMD_TestCommand}
};

#define CommandsTableSize (sizeof(CommandTable)/sizeof(CommandTable[0]))
#endif /* COMMANDS_TABLE_H_ */
