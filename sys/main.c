#include <avr/io.h>
#include <util/delay.h>

#include "common.h"
//#include "tests.h"
#include "comm.h"
#include "uart.h"

#define DELTA 1
void SendDummyCommand(void);

int main (void){

  COMM_Init();
  UINT8 dly = DELTA;
  for (;;)
  {
	  COMM_Update();
	  _delay_ms(10);

	  dly--;
	  if(!dly)
	  {
		  dly = DELTA;
		  SendDummyCommand();
	  }
  }
  return 1;
}

void SendDummyCommand(void)
{
	static UINT16 cnt = 0;
	S_COMMAND cmd;

	cmd.cmd = 18;
	cmd.payload = 2;
	cmd.data[0] = cnt >> 8;
	cmd.data[1] = cnt & 0xFF;

	COMM_SendCommand(&cmd);
	cnt++;
}
