#include <p24FJ128GB206.h>
#include "config.h"
#include "common.h"
#include "pin.h"
#include "ui.h"
#include "timer.h"
#include "stdio.h"

_PIN *coinMech;
int coinState;
int playing = 0;

void coinCheck(void){
	led_on(&led1); //replace with light/ button controls
    // Start timer (legit) or Sketchy possibility of long while loop
     int c = 1, d = 1;
 
   for ( c = 1 ; c <= 3276 ; c++ )
       for ( d = 1 ; d <= 3276 ; d++ )
       {}
    led_off(&led1);
    playing = 0;
}

int16_t main(void) {
    init_clock();
    init_uart();
    init_pin();
    init_ui();
    init_timer();

    coinMech = &D[5];
    pin_digitalIn(coinMech);

    while (1) {
    	coinState = pin_read(coinMech);
    	if (coinState == 0 && playing == 0) {
            playing=1;
    		coinCheck();
    	}
    }
}