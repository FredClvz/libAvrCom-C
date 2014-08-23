//------------------------------------------------------------------------------
//
// Module header : UART_
//
// Description : UART Module Header
//
// F.C.
// 2014.08.16
//
//------------------------------------------------------------------------------

// Multi-inclusion protection
#ifndef UART_H
#define UART_H

// Manages prototyping and declarations
#undef PUBLIC
#ifdef UART_M
#define PUBLIC
#else
#define PUBLIC extern
#endif

//------------------------------------------------------------------------------
//                                                                      Includes
//------------------------------------------------------------------------------
#include "common.h"
#include "ringbuffer.h"

//------------------------------------------------------------------------------
//                                                      Defines & Types exportes
//------------------------------------------------------------------------------
#ifdef CFG_UART_USE_ISR

#ifndef CFG_UART_RX_BUFFER_SIZE
    #define RX_BUFFER_SIZE   20
#else
    #define RX_BUFFER_SIZE   CFG_UART_RX_BUFFER_SIZE
#endif

#ifndef CFG_UART_TX_BUFFER_SIZE
    #define TX_BUFFER_SIZE   20
#else
    #define TX_BUFFER_SIZE   CFG_UART_TX_BUFFER_SIZE
#endif

#endif //CFG_UART_USE_ISR
//------------------------------------------------------------------------------
//                                                           Exported variables
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                                                           Fonctions exportees
//------------------------------------------------------------------------------

PUBLIC void UART_Init(UINT32 u32baudrate);
PUBLIC void UART_Write_u8(const UINT8 input);
PUBLIC void UART_Write_buffer(const UINT8* s, UINT8 n);
PUBLIC void UART_puts(char* s);
PUBLIC void UART_putsln(char* s);
PUBLIC void UART_puthex(const UINT16 val);
PUBLIC void UART_puti(const INT16 val);
PUBLIC Ret_t UART_Read_u8(UINT8* x);
PUBLIC void UART_Flush_rx(void);
PUBLIC RNGB_Buffer* UART_GetRxBuffer(void);

#endif // UART_H
//------------------------------------------------------------------------------
