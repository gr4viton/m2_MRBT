#ifndef DEV_LCD_HD44780_H_INCLUDED
#define DEV_LCD_HD44780_H_INCLUDED

#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/unistd.h>

#include <errno.h>

//#include <libopencm3/cm3/nvic.h>
//#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
//#include <libopencm3/stm32/adc.h>
//#include <libopencm3/stm32/timer.h>

#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>

#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <sys/types.h>

//#include <string.h>

#include "led_f4.h"
//#include "serial.h"
/*
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <sys/types.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
*/
#include "primitives/ringbuf.h"
//#include "leds.h"

//#include "hd44780.h"


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// struct definitions

/*
// define used data_port pins
typedef struct {
    uint16_t D0;
    uint16_t D1;
    uint16_t D2;
    uint16_t D3;
    uint16_t D4;
    uint16_t D5;
    uint16_t D6;
    uint16_t D7;
} lcd_device_data_port_t;
*/
/*
// define used cmd_port pins
typedef struct {
    uint16_t RS;
    uint16_t RW;
    uint16_t EN;
} lcd_device_cmd_port_t;*/

typedef struct {
    //uint32_t device;
    uint32_t cmd_port;
    uint32_t data_port;
    //uint8_t irq;
    //uint8_t af;
    uint32_t cmdp_clk;
    uint32_t datap_clk;

    uint16_t data_pins[8];
    uint16_t data_pins_all;

    uint16_t cmdRS;
    uint16_t cmdRW;
    uint16_t cmdEN;
    uint16_t cmd_pins_all;

    //lcd_device_cmd_port_t cmd;
    //uint32_t txport;
    //uint32_t rxport;
    //uint16_t txpin;
    //uint16_t rxpin;

    struct ringbuf data_ring;
    //struct ringbuf rx_ring;

    uint32_t stats_rxoverruns;
    uint32_t stats_rxerrors;
} lcd_device_t;

//extern lcd_device_t lcds[];

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// function prototypes
FILE *fopenLCD(uint8_t index, uint8_t functionSet, uint8_t entryMode, uint8_t cursorMode,\
               uint8_t *dbuf, size_t dbufsz);
//FILE *fopenlcd(uint8_t index, uint32_t baudrate, uint8_t *tbuf, size_t tbufsz, uint8_t *rbuf, size_t rbufsz);

//struct ringbuf *ser_txbuf(uint8_t index);

// cross reference includes
#include "LCD_HD44780.h"

#endif // DEV_LCD_HD44780_H_INCLUDED
