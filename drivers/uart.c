//------------------------------------------------------------------------------
//
// Module : UART_
//
// Description : UART Module Core
//
// F.C.
// 2014.08.16
//
//------------------------------------------------------------------------------
#define UART_M
#define MOD_NAME "UART"

//------------------------------------------------------------------------------
//                                                                      Includes
//------------------------------------------------------------------------------
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdlib.h>

//#include "ringbuffer.h"

#include "uart.h"


//------------------------------------------------------------------------------
//                                                               Defines & Types
//------------------------------------------------------------------------------
/* Is the UART in a middle of a transmission ? */
#define ISRMODE_IsTransmitting()     (rbi(UCSR0B, UDRIE0))
#define ISRMODE_StartTransmission()  (sbi(UCSR0B, UDRIE0))
#define ISRMODE_EndTransmission()    (cbi(UCSR0B, UDRIE0))
//------------------------------------------------------------------------------
//                                                                     Variables
//------------------------------------------------------------------------------
// Note : OBLIGATOIREMENT static

//IO Buffers when using interrupts
#ifdef CFG_UART_USE_ISR
static VUINT8 sav_RxBuffer[RX_BUFFER_SIZE] = {0};
static VUINT8 sav_TxBuffer[TX_BUFFER_SIZE] = {0};
static volatile RNGB_Buffer sRxBuffer, sTxBuffer;
#endif

//------------------------------------------------------------------------------
//                                             Prototypes des fonctions internes
//------------------------------------------------------------------------------
// Note : OBLIGATOIREMENT static
static inline void _set_baud_rate(UINT32 baud);

//------------------------------------------------------------------------------
//                                                           Fonctions exportees
//------------------------------------------------------------------------------

void UART_Init(UINT32 u32baudrate)
{
  _set_baud_rate(u32baudrate);

  /* baud doubler off  - Only needed on some Uno ...*/
  UCSR0A &= ~(1 << U2X0);

  UCSR0B = (1 << RXEN0)
#if CFG_UART_USE_ISR
         | (1 << RXCIE0)  /* enable RX Complete interrupt */
#endif
         | (1 << TXEN0);

  /* default to 8n1 framing */
  UCSR0C = (3 << 1);

#if CFG_UART_USE_ISR
  RNGB_Init(&sRxBuffer, sav_RxBuffer, RX_BUFFER_SIZE);
  RNGB_Init(&sTxBuffer, sav_TxBuffer, TX_BUFFER_SIZE);
  sei(); //enable interrupts.
#endif
}

/* Base function for writing data to the uart. */
void UART_Write_u8(const UINT8 input)
{
#if CFG_UART_USE_ISR && (TX_BUFFER_SIZE > 0)
  UINT8 data;
  /* Make sure we wont get interrupted while stacking data */
  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    RNGB_Put(&sTxBuffer, input);

    if (!ISRMODE_IsTransmitting()
        && RNGB_Get(&sTxBuffer, &data))
    {
      ISRMODE_StartTransmission();
      UDR0 = data;
    }
  }
#else
    /* wait for transmit buffer to be empty */
    while (!(UCSR0A & (1 << UDRE0))) ;
    UDR0 = input;
#endif
}

void UART_Write_buffer(const UINT8* s, UINT8 n)
{
  for (; n; --n, ++s)
  {
    UART_Write_u8(*s);
  }
}

void UART_puts(char* s)
{
	while(*s)
		UART_Write_u8(*s++);
}

void UART_putsln(char* s)
{
  UART_puts(s);
  UART_puts("\n\r");
}

void UART_puthex(const UINT16 val)
{
    char buffer[8];
    UART_puts(itoa(val, buffer, 16) );
}

void UART_puti(const INT16 val)
{
    char buffer[8];
    UART_puts(itoa(val, buffer, 10) );
}

Ret_t UART_Read_u8(UINT8* x)
{
  /* return non zero on error */
  Ret_t status = RET_OK;

#if CFG_UART_USE_ISR
  if (!RNGB_Get(&sRxBuffer, x))
  {
    status = RET_ERROR;
  }
#else
  while ((UCSR0A & (1 << RXC0)) == 0) ;

  /* Check for Frame Error, Data OverRun or Parity Error */
  if (UCSR0A & ((1 << FE0) | (1 << DOR0) | (1 << UPE0)))
  {
    /* clear errors by reading URD0 */
    status = RET_ERROR;
  }

  *x = UDR0;
#endif
  return status;
}

void UART_Flush_rx(void)
{
  VUINT8 x;

  while (UCSR0A & (1 << RXC0))
  {
    x = UDR0;
    __asm__ __volatile__ ("nop" ::"m"(x));
  }
}

RNGB_Buffer* UART_GetRxBuffer(void)
{
	return &sRxBuffer;
}
//------------------------------------------------------------------------------
//                                                           Fonctions internes
//------------------------------------------------------------------------------

static inline void _set_baud_rate(UINT32 baud)
{
#ifndef CLK_PRESCAL
#define CLK_PRESCAL (1UL)
#endif

  UINT16 x = ((F_CPU / (16UL * CLK_PRESCAL) + baud / 2) / baud - 1);
  UBRR0H = x >> 8;
  UBRR0L = x;
}

//------------------------------------------------------------------------------
//                                                                 Interruptions
//------------------------------------------------------------------------------

/* USART Data Register Empty IT:
 * Doesn't mean that the transmission is complete, but rather that
 * the data register has been transmitted to the UART's shift register.
 */
#if CFG_UART_USE_ISR
#if TX_BUFFER_SIZE > 0
ISR(USART_UDRE_vect)
{
  UINT8 data;

  /* if the buffer is not empty, get a byte and send it. */
  if (RNGB_Get(&sTxBuffer, &data))
  {
    UDR0 = data;
  } else {
    ISRMODE_EndTransmission();
  }
}
#endif

/* USART Receive Complete:
 * A new byte has arrived. Put it in the Rx Ring Buffer.
 * TODO: implement checks around parity error, data overrun, etc...
 */
ISR(USART_RX_vect)
{
  RNGB_Put(&sRxBuffer, UDR0);
}
#endif //CFG_UART_USE_ISR
//------------------------------------------------------------------------------
