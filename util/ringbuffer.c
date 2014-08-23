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
#define RNGB_M

/* Module debug flag.
 * Note: For obvious reasons, this should not be activated while the UART
 * works in ISR mode. */
//#define MOD_DEBUG_ON

//------------------------------------------------------------------------------
//                                                                      Includes
//------------------------------------------------------------------------------
//#define __ASSERT_USE_STDERR
#include <assert.h>
#include "ringbuffer.h"

#if MOD_IsDebugOn
    #include "uart.h"
#endif

//------------------------------------------------------------------------------
//                                                               Defines & Types
//------------------------------------------------------------------------------
#if MOD_IsDebugOn
    #define DEBUG() print_debug_informations(psRB)
#else
    #define DEBUG()
#endif

//------------------------------------------------------------------------------
//                                                                     Variables
//------------------------------------------------------------------------------
// Note : OBLIGATOIREMENT static

//------------------------------------------------------------------------------
//                                             Prototypes des fonctions internes
//------------------------------------------------------------------------------
// Note : OBLIGATOIREMENT static
static void IncIdx(RNGB_Buffer* psRB, UINT8* pIdx);
#if MOD_IsDebugOn
    static void print_debug_informations(RNGB_Buffer* psRB);
#endif

//------------------------------------------------------------------------------
//                                                           Fonctions exportees
//------------------------------------------------------------------------------

/*****************************************************************************
Initialize a circular buffer
Inputs:    -psRB: pointer to a RNGB_Buffer struct.
           -pBuffer: pointer to the data array for the buffer
           -u8Size: maximum number of element the buffer can hold
 *****************************************************************************/
void RNGB_Init(RNGB_Buffer* psRB, bdt* pBuffer, UINT8 u8Size)
{
  psRB->IdxIn = 0;
  psRB->IdxOut = u8Size;
  psRB->u8Size = u8Size;
  psRB->bDataErased = FALSE;
  psRB->pBuffer = pBuffer;
  DEBUG();
}

/* Add an element at the end of the ring buffer */
void RNGB_Put(RNGB_Buffer* psRB, bdt val)
{
  psRB->pBuffer[psRB->IdxIn] = val;

  if (RNGB_IsFull(psRB))
  {
    IncIdx(psRB, &(psRB->IdxOut));
    psRB->bDataErased = TRUE;
  }
  else if(RNGB_IsEmpty(psRB))
  {
    psRB->IdxOut = psRB->IdxIn;
  }

  IncIdx(psRB, &(psRB->IdxIn));

  DEBUG();
}

/* Get an element from the buffer, and remove it.
 * Return True is an element was found. False otherwise. */
BOOL RNGB_Get(RNGB_Buffer* psRB, bdt* pVal)
{
  BOOL bRet;
  if (RNGB_IsEmpty(psRB))
  {
    bRet = FALSE;
  }
  else
  {
    *pVal = psRB->pBuffer[psRB->IdxOut];
    IncIdx(psRB, &psRB->IdxOut);
    if (RNGB_IsFull(psRB)) // we've reached the input by the back, so the buffer is actually empty
    {
      psRB->IdxOut = psRB->u8Size;
    }
    bRet = TRUE;
  }
  DEBUG();
  return bRet;
}

/* Read the buffer without affecting it.
 * index is taken from the start of the buffer (which is IdxOut, not 0) */
BOOL RNGB_Spy(RNGB_Buffer* psRB, UINT8 idx, bdt* pVal)
{
  BOOL bRet;
  UINT8 rIdx;

  if (RNGB_IsEmpty(psRB))
  {
    bRet = FALSE;
  }
  else
  {
    rIdx = (psRB->IdxOut + idx) % psRB->u8Size;
    assert(rIdx >= 0);
    assert(rIdx < psRB->u8Size);

    *pVal = psRB->pBuffer[rIdx];
    bRet = TRUE;
  }
  DEBUG();
  return bRet;
}

/* Is the buffer full? */
inline BOOL RNGB_IsFull(RNGB_Buffer* psRB)
{
  return (psRB->IdxOut == psRB->IdxIn);
}

/* Is the buffer empty? */
inline BOOL RNGB_IsEmpty(RNGB_Buffer* psRB)
{
  return (psRB->IdxOut == psRB->u8Size);
}

/* Returns the number of elements currently in the buffer */
UINT8 RNGB_Count(RNGB_Buffer* psRB)
{
	UINT8 ret;
	if (RNGB_IsEmpty(psRB))
	{
		ret = 0;
	}
	else if (RNGB_IsFull(psRB))
	{
		ret = psRB->u8Size;
	}
	else
	{
		if (psRB->IdxIn > psRB->IdxOut)
			ret = psRB->IdxIn - psRB->IdxOut;
		else
			ret = psRB->u8Size - (psRB->IdxOut - psRB->IdxIn);
	}
	return ret;
}

/* Returns TRUE if some data was erased during a previous Put command
 * and reset the flag.*/
BOOL RNGB_isDataConsistent(RNGB_Buffer* psRB)
{
	BOOL bErased = psRB->bDataErased;
	psRB->bDataErased = FALSE;
	return !bErased;
}
//------------------------------------------------------------------------------
//                                                           Fonctions internes
//------------------------------------------------------------------------------

/* Handles the circular buffer index increment
 * Note: this function should not be called for IdxOut when buffer is empty.*/
static void IncIdx(RNGB_Buffer* psRB, UINT8* pIdx)
{
  (*pIdx)++;
  /* If we reach the end of the buffer, go back to the start*/
  if (*pIdx == psRB->u8Size)
  {
    *pIdx = 0;
  }
}

/* Debug function that prints the content of the buffer alongside the
 * pointers values
 */
#if MOD_IsDebugOn
static void print_debug_informations(RNGB_Buffer* psRB)
{
  UINT8 i;
  bdt* ptr;

  UART_putsln("    RNGB Debug:");

  UART_puts("       base=0x");
  UART_puthex((UINT16)psRB->pBuffer);
  UART_putsln("");

  UART_puts("       pIn=0x");
  UART_puthex((UINT16)psRB->pIn);
  UART_putsln("");

  UART_puts("       pOut=0x");
  UART_puthex((UINT16)psRB->pOut);
  UART_putsln("");

  UART_puts("       N=0x");
  UART_puthex((UINT16)psRB->u8MaxIndex);
  UART_putsln("");

  UART_putsln("       Content= i - 100*val");
  ptr = psRB->pBuffer;
  for(i = 0; i <= psRB->u8MaxIndex; ++i)
  {
    UART_puts("        ");
    UART_puti(i);
    UART_puts(" - ");
    UART_puti((INT16)((*ptr)*100));
    ++ptr;
    UART_putsln("");
  }
  UART_putsln("");
}
#endif
//------------------------------------------------------------------------------
