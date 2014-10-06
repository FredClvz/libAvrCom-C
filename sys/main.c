#include <avr/io.h>
#include <util/delay.h>

#include "common.h"
#include "comm.h"
#include "APP.h"

int main (void)
{
	/* Initialization */
  COMM_Init();

  APP_Init();

  /* Main Loop */
  for (;;)
  {
	  COMM_Update();
	  APP_Main();
  }
  return 1;
}
