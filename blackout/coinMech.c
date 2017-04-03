#include <p24FJ128GB206.h>
#include "config.h"
#include "common.h"
#include "pin.h"
#include "ui.h"
#include "timer.h"
#include "stdio.h"

_PIN *coinMech;
int coinState;

void coinCheck(void){
	led_toggle(&led1);
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
    	if (coinState == 0) {
    		coinCheck();
    	}
    }
}