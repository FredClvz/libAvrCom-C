//------------------------------------------------------------------------------
//
// Module : RNGB_
//
// Description : RingBuffer module
//
// F.C.
// 2014.08.16
//
//------------------------------------------------------------------------------

// Multi-inclusion protection
#ifndef RNGB_H
#define RNGB_H

// Manages prototyping and declarations
#undef PUBLIC
#ifdef RNGB_M
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
typedef UINT8 bdt;  //buffer data type.

typedef struct
{
  UINT8 IdxIn;  //Index of the next free element in the buffer
  UINT8 IdxOut; //IdxOut is set to u8Size when buffer is empty.
  UINT8 u8Size;
  BOOL bDataErased;
  bdt* pBuffer;
}RNGB_Buffer;

//------------------------------------------------------------------------------
//                                                           Exported variables
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                                                           Fonctions exportees
//------------------------------------------------------------------------------

PUBLIC void RNGB_Init(RNGB_Buffer* psRB, bdt* pBuffer, UINT8 u8Size);
PUBLIC void RNGB_Put(RNGB_Buffer* psRB, bdt val);
PUBLIC BOOL RNGB_Get(RNGB_Buffer* psRB, bdt* pVal);
PUBLIC BOOL RNGB_Spy(RNGB_Buffer* psRB, UINT8 idx, bdt* pVal);
PUBLIC BOOL RNGB_IsFull(RNGB_Buffer* psRB);
PUBLIC BOOL RNGB_IsEmpty(RNGB_Buffer* psRB);
PUBLIC UINT8 RNGB_Count(RNGB_Buffer* psRB);
PUBLIC BOOL RNGB_isDataConsistent(RNGB_Buffer* psRB);

#endif // RNGB_H
//------------------------------------------------------------------------------
