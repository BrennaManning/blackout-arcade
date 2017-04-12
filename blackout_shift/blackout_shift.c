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
int gpioa0 = 0;
int prev_gpioa0 = 0;
int diff_gpioa0;
int diff_rising_gpioa0;

int gpiob0 = 0;
int prev_gpiob0 = 0;
int diff_gpiob0;
int diff_rising_gpiob0;

// W0 Register
int olata0 = 0;
int prev_olata0 = 0;

int olatb0 = 0;
int prev_olatb0 = 0;


// R1 Register
int gpioa1 = 0;
int prev_gpioa1 = 0;
int diff_gpioa1;
int diff_rising_gpioa1;

int gpiob1 = 0;
int prev_gpiob1 = 0;
int diff_gpiob1;
int diff_rising_gpiob1;

// W1 Register
int olata1 = 0;
int olatb1 = 0;

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
    init_timer();


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

    spi_open(&spi1, MISO, MOSI, SCK, 1e7, 1);

    pin_clear(RESET);  


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


    timer_setPeriod(&timer2, .01);
    timer_start(&timer2);

    while(1){

        prev_olata0 = olata0;
        prev_olatb0 = olatb0;
        prev_olata1 = olata1;
        prev_olatb1 = olatb1;

        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            led_toggle(&led1);
            prev_gpioa0 = gpioa0;
            prev_gpiob0 = gpiob0;
            prev_gpioa1 = gpioa1;
            prev_gpiob1 = gpiob1;

            gpioa0 = shiftreg_readReg(0x12, CS0); // READ GPIOA PANEL 0 REG 0
            gpiob0 = shiftreg_readReg(0x13, CS0); // READ GPIOB PANEL 0 REG 0
            gpioa1 = shiftreg_readReg(0x12, CS2); // READ GPIOA PANEL 1 REG 2
            gpiob1 = shiftreg_readReg(0x13, CS2); // READ GPIOB PANEL 1 REG 2


            diff_gpioa0 = gpioa0 ^ prev_gpioa0;
            diff_gpiob0 = gpiob0 ^ prev_gpiob0;
            diff_gpioa1 = gpioa1 ^ prev_gpioa1;
            diff_gpiob1 = gpiob1 ^ prev_gpiob1;

            diff_rising_gpioa0 = diff_gpioa0 & gpioa0;
            diff_rising_gpiob0 = diff_gpiob0 & gpiob0;
            diff_rising_gpioa1 = diff_gpioa1 & gpioa1;
            diff_rising_gpiob1 = diff_gpiob1 & gpiob1;

            

            olata0 = olata0 ^ diff_rising_gpioa1;
            olatb0 = olatb0 ^ diff_rising_gpiob1;
            olata1 = olata1 ^ diff_rising_gpioa0;
            olatb1 = olatb1 ^ diff_rising_gpiob0;
            
        }
        

        if (olata0 != prev_olata0){
            shiftreg_writeReg(0x14, olata0, CS1); // WRITE OLATA PANEL 1 REG 3 
        }
        if (olatb0 != prev_olatb0){
            shiftreg_writeReg(0x15, olatb0, CS1); // WRITE OLATB PANEL 1 REG 3            
        }
        if (olata1 != prev_olata1){

        shiftreg_writeReg(0x14, olata1, CS3); // WRITE OLATA PANEL 1 REG 3 
        }

        if (olatb1 != prev_olatb1){
            shiftreg_writeReg(0x15, olatb1, CS3); // WRITE OLATB PANEL 1 REG 3
        }
        
           
    }
}
