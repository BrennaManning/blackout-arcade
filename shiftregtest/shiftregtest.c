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


_PIN *SCK, *MISO, *MOSI, *CS, *RESET, *MISO1, *MOSI1, *CS1;

int b;

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

    pin_clear(CS);
    // Device Opcode = 0b0100A2A1A0 A2 A1 and A0 externally set to 0
    spi_transfer(&spi1, 0b01000000); // DEVICE OPCODE R/W BIT LOW
    spi_transfer(&spi1, address);
    //spi_transfer(&spi1, )
    spi_transfer(&spi1, value);
    pin_set(CS);


}

uint8_t shiftreg_readReg(uint8_t address) {
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


    CS = &D[0];
    SCK = &D[1];
    MOSI = &D[2];
    MISO = &D[3];


    RESET = &D[4];

    CS1 = &D[5];
    MOSI1 = &D[6];
    MISO1 = &D[7];


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
    spi_open(&spi1, MISO1, MOSI1, SCK, 1e6, 1);

    pin_clear(RESET);    
    while(1){
        shiftreg_writeReg(0x0A, 0); // IOCON TO 0
        shiftreg_writeReg(0x0B, 0); // IOCON TO 0
        shiftreg_writeReg(0x00, 0); // IODIRA to 0
        shiftreg_writeReg(0x01, 0); // IODIRB to 0
        shiftreg_writeReg(0x14, 0b10101010); // OLATA
        shiftreg_writeReg(0x15, 0b10101010); // OLATB
        shiftreg_writeReg(0x12, 0b10101010); // GPIOA
        shiftreg_writeReg(0x13, 0b10101010); // GPIOB
        
        // b = shiftreg_readReg(0x13); // GPIOB

        // if ( b== 0b10101010){
        //     led_on(&led1);
        // }
        // else if ( b== 0b11101010){
        //     led_on(&led2);
        // }
        
    }

}
