//------------------------------------------------------------------------------
//
// Module : APP_
//
// Description : Application layer main file.
//
// F.C.
// 2014.10.06
//
//------------------------------------------------------------------------------

// Multi-inclusion protection
#ifndef APP_CMD_H
#define APP_CMD_H

// Manages prototyping and declarations
#undef PUBLIC
#ifdef APP_CMD_M
#define PUBLIC
#else
#define PUBLIC extern
#endif

//------------------------------------------------------------------------------
//                                                                      Includes
//------------------------------------------------------------------------------
#include "common.h"
#include "commands.h"
//------------------------------------------------------------------------------
//                                                      Defines & Types export�s
//------------------------------------------------------------------------------
#define CMD_TEST (0x42)

//------------------------------------------------------------------------------
//                                                           Exported variables
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                                                           Fonctions export�es
//------------------------------------------------------------------------------
PUBLIC void CMD_RX_TestCommand(S_COMMAND* cmd);
PUBLIC void CMD_TX_SendDummyCommand(void);

#endif // APP_H
//------------------------------------------------------------------------------
