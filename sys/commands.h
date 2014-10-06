//------------------------------------------------------------------------------
//
// Module : CMD_
//
// Description : Commands Module Core
//
// F.C.
// 2014.08.22
//
//------------------------------------------------------------------------------

// Multi-inclusion protection
#ifndef CMD_H
#define CMD_H

// Manages prototyping and declarations
#undef PUBLIC
#ifdef CMD_M
#define PUBLIC
#else
#define PUBLIC extern
#endif

//------------------------------------------------------------------------------
//                                                                      Includes
//------------------------------------------------------------------------------
#include "common.h"

//------------------------------------------------------------------------------
//                                                      Defines & Types exportes
//------------------------------------------------------------------------------
typedef enum
{
	COMMAND_EMPTY = 0,  //No new command, or previous command rejected (bad crc?)
	COMMAND_NEW,        //Command needs to be checked first (CRC)
	COMMAND_CHECKED,    //Command is OK. Waiting for treatment.
} E_COMMAND_STATUS;

typedef struct
{
	E_COMMAND_STATUS eCommandStatus;
	UINT8 cmd;
	UINT8 payload;
	UINT8 data[CFG_COMM_MAX_CMD_DATA];
	UINT8 crc[CFG_COMM_CRC_LEN];
} S_COMMAND;

//------------------------------------------------------------------------------
//                                                           Exported variables
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                                                           Fonctions exportees
//------------------------------------------------------------------------------

PUBLIC Ret_t CMD_Execute(S_COMMAND* cmd);
PUBLIC void CMD_TestCommand(S_COMMAND* cmd);

#endif // CMD_H
//------------------------------------------------------------------------------
