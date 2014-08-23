/* Test disabled. To re-enable it, change the buffer data type (bdt typedef)
 * in the ringbuffer.h
 */

///*
// * test_ringbuff.c
// *
// * A simple test for the ring buffer.
// * Note: the MOD_DEBUG_ON should be defined in ringbuffer.c
// */
//#include "ringbuffer.h"
//#include "uart.h"
//
//#define N_VAL 10
//
//void test_ringbuffer(void)
//{
//  RNGB_Buffer buff;
//  FLOAT32 buffdata[N_VAL] = {0};
//
//  BOOL status;
//  FLOAT32 val;
//  int i;
//
//  //Init:
//  UART_Init(CFG_UART_BAUDRATE);
//  UART_putsln("Initializing the buffer.");
//  RNGB_Init(&buff, buffdata, N_VAL);
//
//  //Try to get an element
//  UART_putsln("Try getting an element from an empty buffer");
//  status = RNGB_Get(&buff, &val);
//  UART_puts("Gotten an element? ");
//  UART_puthex(status);
//  UART_putsln("");
//
//  //Try putting an elt
//  UART_putsln("Try putting two elements to an empty buffer");
//  RNGB_Put(&buff, 42.0f);
//  RNGB_Put(&buff, 43.0f);
//
//  //Try to get an element
//  UART_putsln("Try getting an element");
//  status = RNGB_Get(&buff, &val);
//  UART_puts("Gotten an element? ");
//  UART_puthex(status);
//  UART_putsln("");
//
//  //Try putting 12 elts
//  UART_putsln("Try putting 12 elements");
//  for(i = 0; i<=12; ++i)
//  {
//    RNGB_Put(&buff, (FLOAT32)10 * i);
//  }
//
//}
//
