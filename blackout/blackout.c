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
_PIN *bP1_3;
_PIN *bP2_3;
_PIN *bP1_4;
_PIN *bP2_4;
_PIN *ledP1; 
_PIN *ledP2;
_PIN *ledP1_2; 
_PIN *ledP2_2;
_PIN *ledP1_3; 
_PIN *ledP2_3; 
_PIN *ledP1_4; 
_PIN *ledP2_4;  

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

int leds_p1_3; 
int buttons_p1_3;
int prev_buttons_p1_3;
int diff_buttons_p1_3; 
int diff_rising_buttons_p1_3;

int leds_p1_4; 
int buttons_p1_4;
int prev_buttons_p1_4;
int diff_buttons_p1_4; 
int diff_rising_buttons_p1_4;

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

int leds_p2_3;
int buttons_p2_3;
int prev_buttons_p2_3;
int diff_buttons_p2_3;
int diff_rising_buttons_p2_3;

int leds_p2_4;
int buttons_p2_4;
int prev_buttons_p2_4;
int diff_buttons_p2_4;
int diff_rising_buttons_p2_4;

int prev_leds_p1;
int prev_leds_p2;

int prev_leds_p1_2;
int prev_leds_p2_2;

int prev_leds_p1_3;
int prev_leds_p2_3;

int prev_leds_p1_4;
int prev_leds_p2_4;

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

        else if (buttonNum == 3){
            return pin_read(bP1_3);
        }

        else if (buttonNum == 4){
            return pin_read(bP1_4);
        }
        
    }
    else if (panel == 2){
        if (buttonNum == 1){
            return pin_read(bP2);
        }
        
        else if (buttonNum == 2){
            return pin_read(bP2_2);
        }

        else if (buttonNum == 3){
            return pin_read(bP2_3);
        }

        else if (buttonNum == 4){
            return pin_read(bP2_4);
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
        else if (lightNum == 3){   
            pin_write(ledP1_3, leds);
        }
        else if (lightNum == 4){   
            pin_write(ledP1_4, leds);
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
        else if (lightNum == 3){
            pin_write(ledP2_3, leds);
        }
        else if (lightNum == 4){
            pin_write(ledP2_4, leds);
        }
    }
}

int16_t main(void) {
    init_clock();
    init_uart();
    init_pin();
    init_ui();
    init_timer();

    bP1 = &D[8];
    bP1_2 = &D[9];
    bP1_3 = &D[10];
    bP1_4 = &D[11];
    ledP1 = &A[0];
    ledP1_2 = &A[1];
    ledP1_3 = &A[2];
    ledP1_4 = &A[3];

    bP2 = &D[0];
    bP2_2 = &D[1];
    bP2_3 = &D[2];
    bP2_4 = &D[3];
    ledP2 = &D[4];
    ledP2_2 = &D[5];
    ledP2_3 = &D[6];
    ledP2_4 = &D[7];

    pin_digitalIn(bP1);
    pin_digitalIn(bP2);

    pin_digitalIn(bP1_2);
    pin_digitalIn(bP2_2);

    pin_digitalIn(bP1_3);
    pin_digitalIn(bP2_3);

    pin_digitalIn(bP1_4);
    pin_digitalIn(bP2_4);

    pin_digitalOut(ledP1);
    pin_digitalOut(ledP2);

    pin_digitalOut(ledP1_2);
    pin_digitalOut(ledP2_2);

    pin_digitalOut(ledP1_3);
    pin_digitalOut(ledP2_3);

    pin_digitalOut(ledP1_4);
    pin_digitalOut(ledP2_4);

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

    leds_p1_3 = 0x0000; 
    buttons_p1_3 = 0x0000;
    prev_buttons_p1_3 = 0x0000;
    diff_buttons_p1_3 = 0x0000; 
    diff_rising_buttons_p1_3 = 0x0000;

    leds_p1_4 = 0x0000; 
    buttons_p1_4 = 0x0000;
    prev_buttons_p1_4 = 0x0000;
    diff_buttons_p1_4 = 0x0000; 
    diff_rising_buttons_p1_4 = 0x0000;

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

    leds_p2_3 = 0x0000;
    buttons_p2_3 = 0x0000;
    prev_buttons_p2_3 = 0x0000;
    diff_buttons_p2_3 = 0x0000;
    diff_rising_buttons_p2_3 = 0x0000;

    leds_p2_4 = 0x0000;
    buttons_p2_4 = 0x0000;
    prev_buttons_p2_4 = 0x0000;
    diff_buttons_p2_4 = 0x0000;
    diff_rising_buttons_p2_4 = 0x0000;

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

            prev_buttons_p1_3 = buttons_p1_3;
            buttons_p1_3 = readButtons(1, 3);
            diff_buttons_p1_3 = buttons_p1_3 ^ prev_buttons_p1_3;
            diff_rising_buttons_p1_3 = diff_buttons_p1_3 & buttons_p1_3;

            prev_buttons_p1_4 = buttons_p1_4;
            buttons_p1_4 = readButtons(1, 4);
            diff_buttons_p1_4 = buttons_p1_4 ^ prev_buttons_p1_4;
            diff_rising_buttons_p1_4 = diff_buttons_p1_4 & buttons_p1_4;

            prev_buttons_p2 = buttons_p2;
            buttons_p2 = readButtons(2, 1);
            diff_buttons_p2 = buttons_p2 ^ prev_buttons_p2;
            diff_rising_buttons_p2 = diff_buttons_p2 & buttons_p2;

            prev_buttons_p2_2 = buttons_p2_2;
            buttons_p2_2 = readButtons(2, 2);
            diff_buttons_p2_2 = buttons_p2_2 ^ prev_buttons_p2_2;
            diff_rising_buttons_p2_2 = diff_buttons_p2_2 & buttons_p2_2;

            prev_buttons_p2_3 = buttons_p2_3;
            buttons_p2_3 = readButtons(2, 3);
            diff_buttons_p2_3 = buttons_p2_3 ^ prev_buttons_p2_3;
            diff_rising_buttons_p2_3 = diff_buttons_p2_3 & buttons_p2_3;

            prev_buttons_p2_4 = buttons_p2_4;
            buttons_p2_4 = readButtons(2, 4);
            diff_buttons_p2_4 = buttons_p2_4 ^ prev_buttons_p2_4;
            diff_rising_buttons_p2_4 = diff_buttons_p2_4 & buttons_p2_4;

            prev_leds_p1 = leds_p1;
            prev_leds_p2 = leds_p2;

            prev_leds_p1_2 = leds_p1_2;
            prev_leds_p2_2 = leds_p2_2;
            
            prev_leds_p1_3 = leds_p1_3;
            prev_leds_p2_3 = leds_p2_3;

            prev_leds_p1_4 = leds_p1_4;
            prev_leds_p2_4 = leds_p2_4;

            leds_p1 = leds_p1 ^ diff_rising_buttons_p2;
            leds_p2 = leds_p2 ^ diff_rising_buttons_p1;

            leds_p1_2 = leds_p1_2 ^ diff_rising_buttons_p2_2;
            leds_p2_2 = leds_p2_2 ^ diff_rising_buttons_p1_2;

            leds_p1_3 = leds_p1_3 ^ diff_rising_buttons_p2_3;
            leds_p2_3 = leds_p2_3 ^ diff_rising_buttons_p1_3;

            leds_p1_4 = leds_p1_4 ^ diff_rising_buttons_p2_4;
            leds_p2_4 = leds_p2_4 ^ diff_rising_buttons_p1_4;

            if (leds_p1 != prev_leds_p1 && diff_rising_buttons_p2 != 0){

                writeLights(1, leds_p1, 1);

            }

            if (leds_p1_2 != prev_leds_p1_2 && diff_rising_buttons_p2_2 != 0){

                writeLights(1, leds_p1_2, 2);

            }

            if (leds_p1_3 != prev_leds_p1_3 && diff_rising_buttons_p2_3 != 0){

                writeLights(1, leds_p1_3, 3);

            }

            if (leds_p1_4 != prev_leds_p1_4 && diff_rising_buttons_p2_4 != 0){

                writeLights(1, leds_p1_4, 4);

            }

            if (leds_p2 != prev_leds_p2 && diff_rising_buttons_p1 != 0){

                writeLights(2, leds_p2, 1);

            }

            if (leds_p2_2 != prev_leds_p2_2 && diff_rising_buttons_p1_2 != 0){

                writeLights(2, leds_p2_2, 2);

            }

            if (leds_p2_3 != prev_leds_p2_3 && diff_rising_buttons_p1_3 != 0){

                writeLights(2, leds_p2_3, 3);

            }

            if (leds_p2_4 != prev_leds_p2_4 && diff_rising_buttons_p1_4 != 0){

                writeLights(2, leds_p2_4, 4);

            }

        }

    }
}