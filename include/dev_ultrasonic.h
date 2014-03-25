#ifndef DEV_ULTRASONIC_H_INCLUDED
#define DEV_ULTRASONIC_H_INCLUDED

/*
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <sys/types.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#include <stdio.h>
#include <sys/unistd.h>
*/

/*
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>
*/


#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>




double ULTRA_getDist(uint8_t index);


#endif // DEV_ULTRASONIC_H_INCLUDED
