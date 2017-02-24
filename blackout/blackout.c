#include <p24FJ128GB206.h>
#include "config.h"
#include "common.h"
#include "pin.h"
#include "ui.h"
#include "timer.h"
#include "oc.h"


typedef void (*STATE_HANDLER_T)(void);

void on(void); // button1 on
void off(void); // button1 off
int button1 = 0;
int lastbutton1 = 0;

STATE_HANDLER_T state, last_state;

void on(void) {
    if (state != last_state ) {
        pin_set(&D[4]); //light on 
        last_state = state;
        state = state;
    }
    
    // else if (button1 == 1 && button1 != lastbutton1) { 
    //     led_off(&led1);
    //     last_state = state;
    //     state = off;
    // }

    else if (button1 == 1) {
        led_off(&led3);
    }

    else if (button1 != lastbutton1){
        led_on(&led2);
    }

    
}

void off(void) {
    if (state != last_state) {
        led_on(&led3); 
        pin_clear(&D[4]); // light off
        last_state = state;
        state = state;
    }
    
    else if (button1 == 1  && button1 != lastbutton1) {
        led_on(&led1);
        last_state = state;
        state = on;


    }
}


int16_t main(void) {
    init_clock();
    init_uart();
    init_pin();
    init_ui();
    //init_oc();
    pin_digitalIn(&D[0]); // button pressed/ unpressed
    pin_digitalOut(&D[4]); // light on/ off
    //oc_pwm(&oc1, &D[13], NULL, 10e3, 0x8000);

    state = off;
    last_state = (STATE_HANDLER_T)NULL;

    while (1) {
        lastbutton1 = button1;
        button1 = pin_read(&D[0]); // check if button pressed
        state();
    //     button1 = pin_read(&D[0]);
    //     if (button1 == 1){
    //         led_on(&led2);
    //         pin_set(&D[4]);
    //     }
    
    //     else{
    //         led_off(&led2);
    //         pin_clear(&D[4]);
    //     }
    // }
    }
}

