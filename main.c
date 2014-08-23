#include <avr/io.h>
#include <util/delay.h>

#include "common.h"
//#include "tests.h"
#include "comm.h"
#include "uart.h"

int main (void){
  // test_basic();
  COMM_Init();

  for (;;)
  {
	  COMM_Update();
	  _delay_ms(100);

  }
  return 1;
}
