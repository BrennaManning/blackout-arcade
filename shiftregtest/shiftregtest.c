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


_PIN *SCK, *MISO, *MOSI, *CS;


// PIN NUMBERS
// CS &D[3]
// SCK &D[2]
// MOSI &D[1]
// MISO &D[0]


// ADDRESSES
// GPIOA 0x12 
// IODIRA 0x00
// OLATA 0x14

// GPIOB 0x13
// IODIRB 0x01
// OLATB 0x15



void shiftreg_writeReg(uint8_t address, uint8_t value) {
    if (address<=0x7E) {
        led_on(&led3);
        pin_clear(CS);
        spi_transfer(&spi1, address);
        spi_transfer(&spi1, value);
        pin_set(CS);
    }
}

uint8_t shiftreg_readReg(uint8_t address) {
    uint8_t value;

    if (address<=0x7E) {
        pin_clear(CS);
        spi_transfer(&spi1, 0x80|address);
        value = spi_transfer(&spi1, 0);
        pin_set(CS);
        return value;
    } else
        return 0xFF;
}


int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_spi();


    CS = &D[3];
    SCK = &D[2];
    MOSI = &D[1];
    MISO = &D[0];

    // pin_init(SCK, (uint16_t *)&PORTB, (uint16_t *)&TRISB, 
    //          (uint16_t *)&ANSB, 9, 9, 8, 9, (uint16_t *)&RPOR4);
    // pin_init(MISO, (uint16_t *)&PORTB, (uint16_t *)&TRISB, 
    //          (uint16_t *)&ANSB, 14, 14, 0, 14, (uint16_t *)&RPOR7);
    // pin_init(MOSI, (uint16_t *)&PORTB, (uint16_t *)&TRISB, 
    //          (uint16_t *)&ANSB, 8, 8, 0, 8, (uint16_t *)&RPOR4);

    // pin_init(CS, (uint16_t *)&PORTB, (uint16_t *)&TRISB, 
    //          (uint16_t *)NULL, 13, -1, 0, -1, (uint16_t *)NULL);


    pin_digitalOut(CS);
    pin_set(CS);
    
    while(1){

        spi_open(&spi1, MISO, MOSI, SCK, 1e6, 1);
        shiftreg_writeReg(0x0A, 0); // IOCON BANK TO 0
        shiftreg_writeReg(0x15, 0b11111111); // OLAT
        shiftreg_writeReg(0x13, 0b11111111); // GPIOB
    }

}
