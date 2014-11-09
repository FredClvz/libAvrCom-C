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
#include "commands.h"

//------------------------------------------------------------------------------
//                                                 Command definitions inclusion
//------------------------------------------------------------------------------
#include "CMD.h"

//------------------------------------------------------------------------------


/* A command pointed to should have a definition similar to the following:
 * void function_name (S_COMMAND*)
 *
 * To optimize the code execution, a good practice is to put the most used
 * callbacks at the start of the table, so that the command callback algorithm
 * finds them rapidly (i.e. without having to walk the whole table)
 */
const S_CommandTableElt CommandTable[] =
{
//   Cmd ID,   Callback pointer
	{CMD_TEST, &CMD_RX_TestCommand}
};

#define CommandsTableSize (sizeof(CommandTable)/sizeof(CommandTable[0]))  //Implies that there is at least one element in the table ...
#endif /* COMMANDS_TABLE_H_ */
