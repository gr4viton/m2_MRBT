#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


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
#include "dev_serial.h"

// LCD
#include "dev_LCD_HD44780.h"
#include "LCD_HD44780.h"
#include "dev_ultrasonic.h"


#endif // MAIN_H_INCLUDED
