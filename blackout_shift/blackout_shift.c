#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
//#include "usb.h"
#include "pin.h"
#include "spi.h"
#include "timer.h"
#include "oc.h"
#include <stdio.h>
#include <stdlib.h>


// Pins
_PIN *SCK, *MISO, *MOSI, *RESET, *CS0, *CS1, *CS2, *CS3, *coinMech;

// Coin Mech/Game start state constants
int coinState;
int playing = 0;

// R0 Register
int gpioa0 = 0b00000000;
int prev_gpioa0 = 0b00000000;
int diff_gpioa0;
int diff_rising_gpioa0;

int gpiob0 = 0b00000000;
int prev_gpiob0 = 0b00000000;
int diff_gpiob0;
int diff_rising_gpiob0;

// W0 Register
int olata0 = 0b00000000;
int prev_olata0 = 0b00000000;

int olatb0 = 0b00000000;
int prev_olatb0 = 0b00000000;


// R1 Register
int gpioa1 = 0b00000000;
int prev_gpioa1 = 0b00000000;
int diff_gpioa1;
int diff_rising_gpioa1;

int gpiob1 = 0b00000000;
int prev_gpiob1 = 0b00000000;
int diff_gpiob1;
int diff_rising_gpiob1;

// W1 Register
int olata1 = 0b00000000;
int olatb1 = 0b00000000;

int prev_olata1 = 0b00000000;
int prev_olatb1 = 0b00000000;

// For lightsRandom
int random_olat;
int random_bit;
int olat;
int new_olat;

// For Game Start
int start_counter = 0;

// For Game End
int end_counter = 0;

void shiftreg_writeReg(uint8_t address, uint8_t value, struct _PIN * CS) {
    // Write values to register
    // address = adress in register you are writing values to
    // value = value you are writing 
    // CS = Chip select pin corresponding to the shift register chip you want to write to
    pin_clear(CS);
    // Device Opcode = 0b0100A2A1A0 A2 A1 and A0 externally set to 0
    spi_transfer(&spi1, 0b01000000); // DEVICE OPCODE R/W BIT LOW
    spi_transfer(&spi1, address);
    //spi_transfer(&spi1, )
    spi_transfer(&spi1, value);
    pin_set(CS);
}

uint8_t shiftreg_readReg(uint8_t address, struct _PIN * CS) {
     // Read values from register
    // address = adress in register you are reading a value from
    // CS = Chip select pin corresponding to the shift register chip you want to read from
    uint8_t value;

    pin_clear(CS);
    // Device Opcode = 0b0100A2A1A0 A2 A1 and A0 externally set to 0
    spi_transfer(&spi1, 0b01000001); // DEVICE OPCODE R/W BIT HIGH
    spi_transfer(&spi1, address);
    value = spi_transfer(&spi1, 0);
    pin_set(CS);
    return value;
   
}


int lightsRandom(olat){
    // Randomly turns on a light 
    int random_bit = (rand() % 8); // choose random integer 0-7
    if (random_bit == 0){
        led_toggle(&led2);
    }
    new_olat = olat | (1 << random_bit);  // if bit corresponding to random_bit integer is low, set it high.
    return new_olat;
}


void coinCheck(void){
    led_on(&led2); //replace with light/ button controls
    // Start timer (legit) or Sketchy possibility of long while loop
     int c = 1, d = 1;
 
   for ( c = 1 ; c <= 3276 ; c++ )
       for ( d = 1 ; d <= 3276 ; d++ )
       {}
    led_off(&led2);
    playing = 0;
}


void gameStart(struct _PIN * CS1, struct _PIN *CS3){
    timer_setPeriod(&timer3, 1);
    timer_start(&timer3);


    // GAME START
    while(start_counter <= 4){
        if (timer_flag(&timer3)){
            timer_lower(&timer3);
            if (start_counter == 0){
                olata0 = 0b11110000;
                olatb0 = 0b00000000;
                olata1 = 0b11110000;
                olatb1 = 0b00000000;
            }
            else if (start_counter == 1){
                olata0 = 0b11111111;
                olatb0 = 0b00000000;
                olata1 = 0b11111111;
                olatb1 = 0b00000000;
            }
            else if (start_counter == 2){
                olata0 = 0b11111111;
                olatb0 = 0b11110000;
                olata1 = 0b11111111;
                olatb1 = 0b11110000;

            }
            else if (start_counter == 3){
                olata0 = 0b11111111;
                olatb0 = 0b11111111;
                olata1 = 0b11111111;
                olatb1 = 0b11111111;

            }
            else if (start_counter == 4){
                olata0 = 0b00000000;
                olatb0 = 0b00000000;
                olata1 = 0b00000000;
                olatb1 = 0b00000000;

            }
            shiftreg_writeReg(0x14, olata0, CS1);
            shiftreg_writeReg(0x15, olatb0, CS1);
            shiftreg_writeReg(0x14, olata1, CS3);
            shiftreg_writeReg(0x15, olatb1, CS3);
            start_counter += 1;
        }
    }
    start_counter = 0;

}


void gameEnd(struct _PIN * CS1, struct _PIN *CS3){
    timer_setPeriod(&timer3, 1);
    timer_start(&timer3);


    // GAME END
    while(end_counter <= 8){
        if (timer_flag(&timer3)){
            timer_lower(&timer3);
             if (end_counter % 2 == 0){
                olata0 = 0b00000000;
                olatb0 = 0b00000000;
                olata1 = 0b00000000;
                olatb1 = 0b00000000;
            }
            else {
                olata0 = 0b11111111;
                olatb0 = 0b11111111;
                olata1 = 0b11111111;
                olatb1 = 0b11111111;
            }
            shiftreg_writeReg(0x14, olata0, CS1);
            shiftreg_writeReg(0x15, olatb0, CS1);
            shiftreg_writeReg(0x14, olata1, CS3);
            shiftreg_writeReg(0x15, olatb1, CS3);
            end_counter += 1;
        }
    }
    end_counter = 0;

}

void playGame(struct _PIN * CS0, struct _PIN * CS1, struct _PIN * CS2, struct _PIN * CS3){
    // Main gameplay loop.
    // Inputs CS0, CS1, CS2,CS3 are chip select pins for each shift register chip

    srand(time(NULL));       // initialize random function    

    led_on(&led3);       
    playing = 1;

    
    // Initialize timers
    timer_setPeriod(&timer2, .01);
    timer_start(&timer2);
    timer_setPeriod(&timer3, 1);
    timer_start(&timer3);




    while(playing = 1){
        // light values are written to olats each loop.
        // save previous loop's values to prev_olat_
        prev_olata0 = olata0;
        prev_olatb0 = olatb0;
        prev_olata1 = olata1;
        prev_olatb1 = olatb1;

        // If it's time to check for buttons
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            // button states are saved as gpio_ 
            // set previous gpio_ to last loop's values
            prev_gpioa0 = gpioa0;
            prev_gpiob0 = gpiob0;
            prev_gpioa1 = gpioa1;
            prev_gpiob1 = gpiob1;

            // read new values
            gpioa0 = shiftreg_readReg(0x12, CS0); // READ GPIOA PANEL 0 REG 0
            gpiob0 = shiftreg_readReg(0x13, CS0); // READ GPIOB PANEL 0 REG 0
            gpioa1 = shiftreg_readReg(0x12, CS2); // READ GPIOA PANEL 1 REG 2
            gpiob1 = shiftreg_readReg(0x13, CS2); // READ GPIOB PANEL 1 REG 2

            // difference between new value and last value
            diff_gpioa0 = gpioa0 ^ prev_gpioa0;
            diff_gpiob0 = gpiob0 ^ prev_gpiob0;
            diff_gpioa1 = gpioa1 ^ prev_gpioa1;
            diff_gpiob1 = gpiob1 ^ prev_gpiob1;

            // only bits going from high to low set high (was button pressed)
            diff_rising_gpioa0 = diff_gpioa0 & gpioa0;
            diff_rising_gpiob0 = diff_gpiob0 & gpiob0;
            diff_rising_gpioa1 = diff_gpioa1 & gpioa1;
            diff_rising_gpiob1 = diff_gpiob1 & gpiob1;

            
            // if button pressed, corresponding light bit set high
            olata0 = olata0 ^ diff_rising_gpioa1;
            olatb0 = olatb0 ^ diff_rising_gpiob1;
            olata1 = olata1 ^ diff_rising_gpioa0;
            olatb1 = olatb1 ^ diff_rising_gpiob0;
            
        }

        // if it is time to randomly turn on a light
        if (timer_flag(&timer3)){
            timer_lower(&timer3);

            int random_olat = (rand() % 4); // choose random integer 0-3 (choose random set of 8 buttons from this)
            // After set of buttons selected, call lightsRandom on that olat to set a random bit high
            if (random_olat == 0){
                olata0 = lightsRandom(olata0);
            }
            else if (random_olat == 1){
                olatb0 = lightsRandom(olatb0);
            }
            else if (random_olat == 2){
                olata1 = lightsRandom(olata1);
            }
            else if (random_olat == 3){
                olatb1 = lightsRandom(olatb1);
            }

        }
        
        // if olat has changed, write to the register
        if (olata0 != prev_olata0){
            shiftreg_writeReg(0x14, olata0, CS1); // WRITE OLATA PANEL 0 REG 1 
        }
        if (olatb0 != prev_olatb0){
            shiftreg_writeReg(0x15, olatb0, CS1); // WRITE OLATB PANEL 0 REG 1            
        }
        if (olata1 != prev_olata1){

        shiftreg_writeReg(0x14, olata1, CS3); // WRITE OLATA PANEL 1 REG 3 
        }
        if (olatb1 != prev_olatb1){
            shiftreg_writeReg(0x15, olatb1, CS3); // WRITE OLATB PANEL 1 REG 3
        }

         //GAME OVER
        if (olata0 == 255 && olatb0 == 255){
            led_on(&led1);
            gameEnd(CS1, CS3);
            playing = 0;

        }
        if  (olata1 == 255 && olatb1 == 255){
            led_on(&led1);
            gameEnd(CS1, CS3);
            playing = 0;
        }
           
    }
    led_off(&led1);
}


void shiftreg_config(struct _PIN * CS0, struct _PIN * CS1, struct _PIN * CS2, struct _PIN * CS3) {

    shiftreg_writeReg(0x0A, 0, CS0); // IOCON TO 0
    shiftreg_writeReg(0x0B, 0, CS0); // IOCON TO 0
    shiftreg_writeReg(0x00, 0, CS0); // IODIRA to 0
    shiftreg_writeReg(0x01, 0, CS0); // IODIRB to 0
    
    shiftreg_writeReg(0x0A, 0, CS1); // IOCON TO 0
    shiftreg_writeReg(0x0B, 0, CS1); // IOCON TO 0
    shiftreg_writeReg(0x00, 0, CS1); // IODIRA to 0
    shiftreg_writeReg(0x01, 0, CS1); // IODIRB to 0

    shiftreg_writeReg(0x0A, 0, CS2); // IOCON TO 0
    shiftreg_writeReg(0x0B, 0, CS2); // IOCON TO 0
    shiftreg_writeReg(0x00, 0, CS2); // IODIRA to 0
    shiftreg_writeReg(0x01, 0, CS2); // IODIRB to 0

    shiftreg_writeReg(0x0A, 0, CS3); // IOCON TO 0
    shiftreg_writeReg(0x0B, 0, CS3); // IOCON TO 0
    shiftreg_writeReg(0x00, 0, CS3); // IODIRA to 0
    shiftreg_writeReg(0x01, 0, CS3); // IODIRB to 0

}

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_spi();
    init_timer();


    SCK = &D[0];
    MISO = &D[1];
    MOSI = &D[2];
    RESET = &D[3];
    CS0 = &D[4];
    CS1 = &D[5];
    CS2 = &D[6];
    CS3 = &D[7];
    coinMech = &D[8];


    pin_digitalOut(CS0);
    pin_digitalOut(CS1);
    pin_digitalOut(CS2);
    pin_digitalOut(CS3);
    pin_digitalIn(coinMech);


    pin_set(CS0);
    pin_set(CS1);
    pin_set(CS2);
    pin_set(CS3);

    // Start SPI communication
    spi_open(&spi1, MISO, MOSI, SCK, 1e7, 1);

    // clear reset pin
    pin_clear(RESET);  

    shiftreg_config(CS0, CS1, CS2, CS3);

    while (1) {
        led_off(&led3);
        coinState = pin_read(coinMech);
        if (coinState == 0 && playing == 0) {
            playing=1;
            gameStart(CS1, CS3);
            playGame(CS0, CS1, CS2, CS3);
        }
    }
    
}
