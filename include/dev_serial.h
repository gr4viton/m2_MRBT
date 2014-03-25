#ifndef DEV_SERIAL_H_INCLUDED
#define DEV_SERIAL_H_INCLUDED

#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <sys/types.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#include "primitives/ringbuf.h"
//#include "led.h"

#include <stdio.h>
#include <sys/unistd.h>

#include <libopencm3/stm32/gpio.h>

#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>


#include "led_f4.h"
#include "dev_serial.h"


FILE *fopenserial(uint8_t index, uint32_t baudrate, uint8_t *tbuf, size_t tbufsz, uint8_t *rbuf, size_t rbufsz);
//FILE *fopenlcd(uint8_t index, uint32_t baudrate, uint8_t *tbuf, size_t tbufsz, uint8_t *rbuf, size_t rbufsz);

struct ringbuf *ser_txbuf(uint8_t index);

#endif // DEV_SERIAL_H_INCLUDED
