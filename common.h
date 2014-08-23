//------------------------------------------------------------------------------
//
//
// Description : Common inclusion header
//
// F.C.
// 2014.08.16
//
//------------------------------------------------------------------------------

// Multi-inclusion protection
#ifndef COMMON_H
#define COMMON_H


//------------------------------------------------------------------------------
//                                                                      Includes
//------------------------------------------------------------------------------
#include <avr/io.h>
#include "config.h"

//------------------------------------------------------------------------------
//                                                      Defines & Types exportes
//------------------------------------------------------------------------------

//Boolean
typedef enum {FALSE = 0, TRUE = 1}  BOOL;

//Return type
typedef enum {RET_OK = 0, RET_ERROR = 1} Ret_t;

//Arithmetic types
typedef signed char              INT8;
typedef unsigned char            UINT8;
typedef volatile signed char     VINT8;
typedef volatile unsigned char   VUINT8;

typedef signed short             INT16;
typedef unsigned short           UINT16;
typedef volatile signed short    VINT16;
typedef volatile unsigned short  VUINT16;

typedef signed long              INT32;
typedef unsigned long            UINT32;
typedef volatile signed long     VINT32;
typedef volatile unsigned long   VUINT32;

typedef float  FLOAT32;
typedef double FLOAT64;

//Generalities
#ifndef NULL
  #define NULL			(0)
#endif

//------------------------------------------------------------------------------
//                                                                        Macros
//------------------------------------------------------------------------------

/* Debug macro (note: the 'MOD_DEBUG_ON' define should be put at the top of the
 * module's .c file) */
#define MOD_IsDebugOn \
  (!defined(CFG_DISABLE_ALL_DEBUG) \
   && (defined(CFG_ENABLE_ALL_DEBUG) || defined(MOD_DEBUG_ON)))

/* Bit Manipulation */
//Bit Value
#ifndef BV
  #define BV(bit)       (1 << (bit))
#endif
//Clear bit
#ifndef cbi
  #define cbi(reg,bit)  reg &= ~(BV(bit))
#endif
//Set bit
#ifndef sbi
  #define sbi(reg,bit)  reg |= (BV(bit))
#endif
//Read bit
#ifndef rbi
  #define rbi(reg,bit)    ((reg&BV(bit))!=0)
#endif

//------------------------------------------------------------------------------
//                                                           Exported variables
//------------------------------------------------------------------------------


#endif // COMMON_H
//------------------------------------------------------------------------------
