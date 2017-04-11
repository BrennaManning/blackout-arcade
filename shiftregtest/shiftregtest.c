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


_PIN *SCK, *MISO, *MOSI, *CS0, *RESET, *CS1;

int gpiob1;
int olatb1;

int gpiob0;
int olatb0;


void shiftreg_writeReg(uint8_t address, uint8_t value, struct _PIN * CS) {
    if (value == 00000001){
        led_on(&led2);
    }
    else{
        led_off(&led2);
    }
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


    CS0 = &D[4];
    SCK = &D[0];
    MOSI = &D[2];
    MISO = &D[1];


    RESET = &D[3];

    CS1 = &D[5];

    pin_digitalOut(CS0);
    pin_digitalOut(CS1);
    pin_set(CS0);
    pin_set(CS1);
    pin_set(RESET);

    spi_open(&spi1, MISO, MOSI, SCK, 1e6, 1);

    pin_clear(RESET);  


    shiftreg_writeReg(0x0A, 0, CS0); // IOCON TO 0
    shiftreg_writeReg(0x0B, 0, CS0); // IOCON TO 0
    shiftreg_writeReg(0x00, 0, CS0); // IODIRA to 0
    shiftreg_writeReg(0x01, 0, CS0); // IODIRB to 0

    while(1){
        // shiftreg_writeReg(0x14, 0b10101010, CS0); // OLATA
        // shiftreg_writeReg(0x15, 0b10101010, CS0); // olatb1
        // shiftreg_writeReg(0x12, 0b10101010, CS0); // GPIOA
        // shiftreg_writeReg(0x13, 0b10101010, CS0); // GPIOB
        
        // gpiob1 = shiftreg_readReg(0x13, CS1); // GPIOB
        // olatb1 = shiftreg_readReg(0x15, CS1);

        gpiob0 = shiftreg_readReg(0x13, CS0);
        olatb0 = shiftreg_readReg(0x15, CS0);


        if (gpiob0 == 0b00000001){
            led_on(&led1);
        }
        else{
            led_off(&led1);
        }
        
        shiftreg_writeReg(0x14, gpiob0, CS0);
        //shiftreg_writeReg(0x14, gpiob0, CS0);

        
    }

}
