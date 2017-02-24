#include <p24FJ128GB206.h>
#include "config.h"
#include "common.h"
#include "pin.h"
#include "ui.h"
#include "timer.h"
#include "oc.h"


typedef void (*STATE_HANDLER_T)(void);

void on1(void); // button1 on
void off1(void); // button1 off
void on2(void); // button2 on 
void off2(void); // button2 off
int button1 = 0;
int button2 = 0;
int lastbutton1 = 0;
int lastbutton2 = 0;

// int b1Addr = &D[4];

STATE_HANDLER_T state1, last_state1;
STATE_HANDLER_T state2, last_state2;

void on1(void) {
    if (state1 != last_state1) {
        pin_set(&D[4]);
        last_state1 = state1;
    }

    else if (button1 == 1 && button1 != lastbutton1) { 
        led_off(&led1);
        last_state1 = state1;
        state1 = off1;
    }
}

void off1(void) {
    if (state1 != last_state1) {
        // led_on(&led3);
        pin_clear(&D[4]);
        last_state1 = state1;
    }
    
    else if (button1 == 1  && button1 != lastbutton1) {
        led_on(&led1);
        last_state1 = state1;
        state1 = on1;
    }
}

void on2(void) {
    if (state2 != last_state2) {
        pin_set(&D[5]);
        last_state2 = state2;
    }

    else if (button2 == 1 && button2 != lastbutton2) { 
        led_off(&led3);
        last_state2 = state2;
        state2 = off2;
    }
}

void off2(void) {
    if (state2 != last_state2) {
        // led_on(&led3);
        pin_clear(&D[5]);
        last_state2 = state2;
    }
    
    else if (button2 == 1  && button2 != lastbutton2) {
        led_on(&led3);
        last_state2 = state2;
        state2 = on2;
    }
}


int16_t main(void) {
    init_clock();
    init_uart();
    init_pin();
    init_ui();
    init_timer();
    //init_oc();
    pin_digitalIn(&D[0]);
    pin_digitalIn(&D[1]);
    pin_digitalOut(&D[4]);
    pin_digitalOut(&D[5]);
    //oc_pwm(&oc1, &D[13], NULL, 10e3, 0x8000);
    timer_setPeriod(&timer2, 0.15);
    timer_start(&timer2);
    state1 = off1;
    state2 = off2;
    last_state1 = (STATE_HANDLER_T)NULL;
    last_state2 = (STATE_HANDLER_T)NULL;

    while (1) {
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            lastbutton1 = button1;
            lastbutton2 = button2;
            button1 = pin_read(&D[0]);
            button2 = pin_read(&D[1]);
            state1();
            state2();
        }
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

