//------------------------------------------------------------------------------
//
// Module : COMM_
//
// Description : Communication Module Core
//               Manages the communication protocol with the host PC
//
// F.C.
// 2014.08.16
//
//------------------------------------------------------------------------------

// Multi-inclusion protection
#ifndef COMM_H
#define COMM_H

// Manages prototyping and declarations
#undef PUBLIC
#ifdef COMM_M
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
//                                                      Defines & Types exportes
//------------------------------------------------------------------------------
#ifndef CFG_COMM_MAX_CMD_DATA
    #define CFG_COMM_MAX_CMD_DATA 4
#endif

#define COMM_HEADER_1   0x41
#define COMM_HEADER_2   0x67

//------------------------------------------------------------------------------
//                                                           Exported variables
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                                                           Fonctions exportees
//------------------------------------------------------------------------------

PUBLIC void COMM_Init(void);
PUBLIC void COMM_Update(void);
PUBLIC Ret_t COMM_CheckCRC(S_COMMAND* cmd);

#endif // COMM_H
//------------------------------------------------------------------------------
