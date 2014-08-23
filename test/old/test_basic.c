/*
 * test_basic.c
 *
 *  Created on: 16 août 2014
 *      Author: fred
 */


//basic test: makes a led blink (led on pin13 if arduino uno)
#include "common.h"
#include <util/delay.h>
/*
    Wait (100 * 160000) cycles = wait 16000000 cycles.
    Equivalent to 1 second at 16 MHz.
*/
//Prototypes:
void waitASecond();
void test_basic(void);

void test_basic(void)
{
    /* set PORTB PIN7 as output*/
    DDRB = (1<<DDB5);
    while (1){
        /* set PORTB PIN 7 high */
        PORTB |= (1<<PB5);
        waitASecond();
        /* set PORTB PIN7 low */
        PORTB &= ~(1<<PB5);
        waitASecond();
    }
}

void waitASecond(){
    uint16_t i;
    for (i=0; i<10; i++){
        /* wait (40000 x 4) cycles = wait 160000 cycles */
        _delay_loop_2(40000);
    }
}
