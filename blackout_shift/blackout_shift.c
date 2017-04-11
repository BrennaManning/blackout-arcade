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


_PIN *SCK, *MISO, *MOSI, *RESET, *CS0, *CS1, *CS2, *CS3;

// R0 Register
int gpioa0;
int gpiob0;

int prev_gpioa0 = 0;
int prev_gpiob0 = 0;

// W0 Register
int olata0;
int olatb0;

int prev_olata0 = 0;
int prev_olatb0 = 0;


// R1 Register
int gpioa1;
int gpiob1;

int prev_gpioa1 = 0;
int prev_gpiob1 = 0;

// W1 Register
int olata1;
int olatb1;

int prev_olata1 = 0;
int prev_olatb1 = 0;

void shiftreg_writeReg(uint8_t address, uint8_t value, struct _PIN * CS) {

    pin_clear(CS);
    // Device Opcode = 0b0100A2A1A0 A2 A1 and A0 externally set to 0
    spi_transfer(&spi1, 0b01000000); // DEVICE OPCODE R/W BIT LOW
    spi_transfer(&spi1, address);
    //spi_transfer(&spi1, )
    spi_transfer(&spi1, value);
    pin_set(CS);
}

uint8_t shiftreg_readReg(uint8_t address, struct _PIN * CS) {
    uint8_t value;

    pin_clear(CS);
    // Device Opcode = 0b0100A2A1A0 A2 A1 and A0 externally set to 0
    spi_transfer(&spi1, 0b01000001); // DEVICE OPCODE R/W BIT HIGH
    spi_transfer(&spi1, address);
    value = spi_transfer(&spi1, 0);
    pin_set(CS);
    return value;
   
}


int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_spi();


    SCK = &D[0];
    MISO = &D[1];
    MOSI = &D[2];
    RESET = &D[3];
    CS0 = &D[4];
    CS1 = &D[5];
    CS2 = &D[6];
    CS3 = &D[7];


    pin_digitalOut(CS0);
    pin_digitalOut(CS1);
    pin_digitalOut(CS2);
    pin_digitalOut(CS3);


    pin_set(CS0);
    pin_set(CS1);
    pin_set(CS2);
    pin_set(CS3);
    pin_set(RESET);

    spi_open(&spi1, MISO, MOSI, SCK, 1e6, 1);

    pin_clear(RESET);  


    shiftreg_writeReg(0x0A, 0, CS0); // IOCON TO 0
    shiftreg_writeReg(0x0B, 0, CS0); // IOCON TO 0
    shiftreg_writeReg(0x00, 0, CS0); // IODIRA to 0
    shiftreg_writeReg(0x01, 0, CS0); // IODIRB to 0
    
    while(1){

        gpiob0 = shiftreg_readReg(0x13, CS0); // READ GPIOB
        if (gpiob0 & 00000001){
            led_on(&led1);
        }
        if (gpiob0 & 00000010){
            led_on(&led2);
        }
        if (gpiob0 == 0){
            led_on(&led3);
        }
        else{
            led_off(&led3);
        }

        // prev_buttons_p2 = buttons_p2;
        // buttons_p2 = readButtons(2);
        // diff_buttons_p2 = buttons_p2 ^ prev_buttons_p2;
        // diff_rising_buttons_p2 = diff_buttons_p2 & buttons_p2;


        shiftreg_writeReg(0x14, gpiob0, CS0); // WRITE OLATA
        

        shiftreg_writeReg(0x14, gpiob0, CS3);

        // gpioa = shiftreg_readReg(0x12, CS0); // READ GPIOA
        // shiftreg_writeReg(0x15, gpioa, CS0); // WRITE OLATB

        // shiftreg_writeReg(0x15, 0b10101010, CS0); // olatb1
        // shiftreg_writeReg(0x12, 0b10101010, CS0); // GPIOA
        // shiftreg_writeReg(0x13, 0b10101010, CS0); // GPIOB
        
        // gpiob1 = shiftreg_readReg(0x13, CS1); // GPIOB
        // olatb1 = shiftreg_readReg(0x15, CS1);

        // gpiob0 = shiftreg_readReg(0x13, CS0);


        // if (gpiob0 == 0b00000001){
        //     led_on(&led1);
        // }
        // else{
        //     led_off(&led1);
        // }
       
    }


    // while (1) {
    //     if (timer_flag(&timer2)) {
    //         timer_lower(&timer2);
    //         prev_buttons_p1 = buttons_p1;
    //         buttons_p1 = readButtons(1);
    //         diff_buttons_p1 = buttons_p1 ^ prev_buttons_p1;
    //         diff_rising_buttons_p1 = diff_buttons_p1 & buttons_p1;


    //         prev_buttons_p2 = buttons_p2;
    //         buttons_p2 = readButtons(2);
    //         diff_buttons_p2 = buttons_p2 ^ prev_buttons_p2;
    //         diff_rising_buttons_p2 = diff_buttons_p2 & buttons_p2;

    //         prev_leds_p1 = leds_p1;
    //         prev_leds_p2 = leds_p2;
    //         leds_p1 = leds_p1 ^ diff_rising_buttons_p2;
    //         leds_p2 = leds_p2 ^ diff_rising_buttons_p1;

    //         if (leds_p1 != prev_leds_p1 && diff_rising_buttons_p2 != 0){

    //             writeLights(1, leds_p1);

    //         }

    //         if (leds_p2 != prev_leds_p2 && diff_rising_buttons_p1 != 0){

    //             writeLights(2, leds_p2);

    //         }

    //     }

    //     // Randomly turn on a light every 2 seconds
    //     if (timer_flag(&timer3)) {
    //         timer_lower(&timer3);
    //         lightsRandom(leds_p1, leds_p2);
    //     }

    // }

}
