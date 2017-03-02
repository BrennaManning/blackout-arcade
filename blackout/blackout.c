#include <p24FJ128GB206.h>
#include "config.h"
#include "common.h"
#include "pin.h"
#include "ui.h"
#include "timer.h"
#include "oc.h"
#include "spi.h"

_PIN *bP1; 
_PIN *bP2; 
_PIN *ledP1; 
_PIN *ledP2; 

int panel;

int readButtons(int panel);
void writeLights(int panel, int leds);

int leds_p1; 
int buttons_p1;
int prev_buttons_p1;
int diff_buttons_p1; 

int leds_p2;
int buttons_p2;
int prev_buttons_p2;
int diff_buttons_p2;

int prev_leds_p1;
int prev_leds_p2;


int readButtons(panel){
    if (panel == 1){
        return pin_read(bP1);
        // SPI Stuff
        // Clock
        
    }
    else if (panel == 2){
        return pin_read(bP2);
    }
}

void writeLights(panel, leds){
    if (panel == 1){
        pin_write(ledP1, leds);
        // SPI CLOCK STUFF
        led_toggle(&led1);
        
    }
    else if (panel == 2){
        pin_write(ledP2, leds);
        // SPI CLOCK STUFF
        led_toggle(&led2);
    }
}

int16_t main(void) {
    init_clock();
    init_uart();
    init_pin();
    init_ui();
    init_timer();

    bP1 = &D[0];
    bP2 = &D[1];
    ledP1 = &D[2];
    ledP2 = &D[3];

    pin_digitalIn(bP1);
    pin_digitalIn(bP2);
    pin_digitalOut(ledP1);
    pin_digitalOut(ledP2);

    leds_p1 = 0x0000; 
    buttons_p1 = 0x0000;
    prev_buttons_p1 = 0x0000;
    diff_buttons_p1 = 0x0000; 

    leds_p2 = 0x0000;
    buttons_p2 = 0x0000;
    prev_buttons_p2 = 0x0000;
    diff_buttons_p2 = 0x0000;

    timer_setPeriod(&timer2, 0.15);
    timer_start(&timer2);

    while (1) {
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            prev_buttons_p1 = buttons_p1;
            buttons_p1 = readButtons(1);
            diff_buttons_p1 = buttons_p1 ^ prev_buttons_p1;

            prev_buttons_p2 = buttons_p2;
            buttons_p2 = readButtons(2);
            diff_buttons_p2 = buttons_p2 ^ prev_buttons_p2;

            prev_leds_p1 = leds_p1;
            prev_leds_p2 = leds_p2;
            leds_p1 = leds_p1 ^ diff_buttons_p2;
            leds_p2 = leds_p2 ^ diff_buttons_p1;

            if (leds_p1 != prev_leds_p1){

                writeLights(1, leds_p1);

            }

            if (leds_p2 != prev_leds_p2){

                writeLights(2, leds_p2);

            }

        }

    }
}