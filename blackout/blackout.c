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

void lightsRandom(int leds_p1, int leds_p2);

int leds_p1; 
int buttons_p1;
int prev_buttons_p1;
int diff_buttons_p1; 
int diff_rising_buttons_p1;

int leds_p2;
int buttons_p2;
int prev_buttons_p2;
int diff_buttons_p2;
int diff_rising_buttons_p2;

int prev_leds_p1;
int prev_leds_p2;

int random_panel;
int random_bit;
int new_leds;


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

void lightsRandom(leds_p1, leds_p2){
    // Randomly turns on a light
    srand(time(NULL));
    int random_panel = (rand() % 2) + 1;
    int random_bit = rand() % 16;
    if (random_panel == 1){
        new_leds = leds_p1 ^ (1 << random_bit);
    }
    else {
        new_leds = leds_p2 ^ (1 << random_bit);
    }
    writeLights(random_panel, new_leds);
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
    diff_rising_buttons_p1 = 0x0000;

    leds_p2 = 0x0000;
    buttons_p2 = 0x0000;
    prev_buttons_p2 = 0x0000;
    diff_buttons_p2 = 0x0000;
    diff_rising_buttons_p2 = 0x0000;
    timer_setPeriod(&timer2, 0.15);
    timer_start(&timer2);
    timer_setPeriod(&timer3, 2);
    timer_start(&timer3);

    while (1) {
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            prev_buttons_p1 = buttons_p1;
            buttons_p1 = readButtons(1);
            diff_buttons_p1 = buttons_p1 ^ prev_buttons_p1;
            diff_rising_buttons_p1 = diff_buttons_p1 & buttons_p1;


            prev_buttons_p2 = buttons_p2;
            buttons_p2 = readButtons(2);
            diff_buttons_p2 = buttons_p2 ^ prev_buttons_p2;
            diff_rising_buttons_p2 = diff_buttons_p2 & buttons_p2;

            prev_leds_p1 = leds_p1;
            prev_leds_p2 = leds_p2;
            leds_p1 = leds_p1 ^ diff_rising_buttons_p2;
            leds_p2 = leds_p2 ^ diff_rising_buttons_p1;

            if (leds_p1 != prev_leds_p1 && diff_rising_buttons_p2 != 0){

                writeLights(1, leds_p1);

            }

            if (leds_p2 != prev_leds_p2 && diff_rising_buttons_p1 != 0){

                writeLights(2, leds_p2);

            }

        }

        // Randomly turn on a light every 2 seconds
        if (timer_flag(&timer3)) {
            timer_lower(&timer3);
            lightsRandom(leds_p1, leds_p2);
        }

    }
}