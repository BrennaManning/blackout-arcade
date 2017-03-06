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
_PIN *bP1_2;
_PIN *bP2_2;
_PIN *ledP1; 
_PIN *ledP2;
_PIN *ledP1_2; 
_PIN *ledP2_2; 


int panel;
int buttonNum;
int lightNum;

int readButtons(int panel, int buttonNum);
void writeLights(int panel, int leds, int lightNum);

int leds_p1; 
int buttons_p1;
int prev_buttons_p1;
int diff_buttons_p1; 
int diff_rising_buttons_p1;

int leds_p1_2; 
int buttons_p1_2;
int prev_buttons_p1_2;
int diff_buttons_p1_2; 
int diff_rising_buttons_p1_2;

int leds_p2;
int buttons_p2;
int prev_buttons_p2;
int diff_buttons_p2;
int diff_rising_buttons_p2;

int leds_p2_2;
int buttons_p2_2;
int prev_buttons_p2_2;
int diff_buttons_p2_2;
int diff_rising_buttons_p2_2;

int prev_leds_p1;
int prev_leds_p2;

int prev_leds_p1_2;
int prev_leds_p2_2;

int readButtons(panel, buttonNum){
    if (panel == 1){
        if (buttonNum == 1){
            return pin_read(bP1);
        }
        // SPI Stuff
        // Clock
        else if (buttonNum == 2){
            return pin_read(bP1_2);
        }
        
    }
    else if (panel == 2){
        if (buttonNum == 1){
            return pin_read(bP2);
        }
        
        else if (buttonNum == 2){
            return pin_read(bP2_2);
        }
    }
}

void writeLights(panel, leds, lightNum){
    if (panel == 1){
        if (lightNum == 1){   
            pin_write(ledP1, leds);
        }
        else if (lightNum == 2){   
            pin_write(ledP1_2, leds);
        }
        
    }

    else if (panel == 2){
        if (lightNum == 1){
            pin_write(ledP2, leds);
            led_toggle(&led2);
        }
        else if (lightNum == 2){
            pin_write(ledP2_2, leds);
        }
    }
}

int16_t main(void) {
    init_clock();
    init_uart();
    init_pin();
    init_ui();
    init_timer();

    bP1 = &D[0];
    bP1_2 = &D[1];
    bP2 = &D[2];
    bP2_2 = &D[3];
    ledP1 = &D[4];
    ledP1_2 = &D[5];
    ledP2 = &D[6];
    ledP2_2 = &D[7];

    pin_digitalIn(bP1);
    pin_digitalIn(bP2);

    pin_digitalIn(bP1_2);
    pin_digitalIn(bP2_2);

    pin_digitalOut(ledP1);
    pin_digitalOut(ledP2);

    pin_digitalOut(ledP1_2);
    pin_digitalOut(ledP2_2);

    leds_p1 = 0x0000; 
    buttons_p1 = 0x0000;
    prev_buttons_p1 = 0x0000;
    diff_buttons_p1 = 0x0000; 
    diff_rising_buttons_p1 = 0x0000;

    leds_p1_2 = 0x0000; 
    buttons_p1_2 = 0x0000;
    prev_buttons_p1_2 = 0x0000;
    diff_buttons_p1_2 = 0x0000; 
    diff_rising_buttons_p1_2 = 0x0000;

    leds_p2 = 0x0000;
    buttons_p2 = 0x0000;
    prev_buttons_p2 = 0x0000;
    diff_buttons_p2 = 0x0000;
    diff_rising_buttons_p2 = 0x0000;

    leds_p2_2 = 0x0000;
    buttons_p2_2 = 0x0000;
    prev_buttons_p2_2 = 0x0000;
    diff_buttons_p2_2 = 0x0000;
    diff_rising_buttons_p2_2 = 0x0000;


    timer_setPeriod(&timer2, 0.15);
    timer_start(&timer2);

    while (1) {
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);

            prev_buttons_p1 = buttons_p1;
            buttons_p1 = readButtons(1, 1);
            diff_buttons_p1 = buttons_p1 ^ prev_buttons_p1;
            diff_rising_buttons_p1 = diff_buttons_p1 & buttons_p1;

            prev_buttons_p1_2 = buttons_p1_2;
            buttons_p1_2 = readButtons(1, 2);
            diff_buttons_p1_2 = buttons_p1_2 ^ prev_buttons_p1_2;
            diff_rising_buttons_p1_2 = diff_buttons_p1_2 & buttons_p1_2;

            prev_buttons_p2 = buttons_p2;
            buttons_p2 = readButtons(2, 1);
            diff_buttons_p2 = buttons_p2 ^ prev_buttons_p2;
            diff_rising_buttons_p2 = diff_buttons_p2 & buttons_p2;

            prev_buttons_p2_2 = buttons_p2_2;
            buttons_p2_2 = readButtons(2, 2);
            diff_buttons_p2_2 = buttons_p2_2 ^ prev_buttons_p2_2;
            diff_rising_buttons_p2_2 = diff_buttons_p2_2 & buttons_p2_2;

            prev_leds_p1 = leds_p1;
            prev_leds_p2 = leds_p2;

            prev_leds_p1_2 = leds_p1_2;
            prev_leds_p2_2 = leds_p2_2;
            

            leds_p1 = leds_p1 ^ diff_rising_buttons_p2;
            leds_p2 = leds_p2 ^ diff_rising_buttons_p1;

            leds_p1_2 = leds_p1_2 ^ diff_rising_buttons_p2_2;
            leds_p2_2 = leds_p2_2 ^ diff_rising_buttons_p1_2;

            if (leds_p1 != prev_leds_p1 && diff_rising_buttons_p2 != 0){

                writeLights(1, leds_p1, 1);

            }

            if (leds_p1_2 != prev_leds_p1_2 && diff_rising_buttons_p2_2 != 0){

                writeLights(1, leds_p1_2, 2);

            }

            if (leds_p2 != prev_leds_p2 && diff_rising_buttons_p1 != 0){

                writeLights(2, leds_p2, 1);

            }

            if (leds_p2_2 != prev_leds_p2_2 && diff_rising_buttons_p1_2 != 0){

                writeLights(2, leds_p2_2, 2);

            }

        }

    }
}