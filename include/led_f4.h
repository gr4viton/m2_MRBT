#ifndef LED_F4_H_INCLUDED
#define LED_F4_H_INCLUDED

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// includes
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// defines

#define PLED        GPIOD
#define RCC_PLED    RCC_GPIOD


#define LED0 GPIO12
#define LED1 GPIO13
#define LED2 GPIO14
#define LED3 GPIO15

#define LEDS (LED0|LED1|LED2|LED3)
#define LED_ALL LEDS

#define LEDGREEN0 LED0
#define LEDORANGE1 LED1
#define LEDRED2 LED2
#define LEDBLUE3 LED3

/*
#define LED_ON(led)     gpio_clear(PLED, (led))
#define LED_OFF(led)    gpio_set(PLED, (led))
#define LED_TGL(led)    gpio_toggle(PLED, (led))
*/

void INIT_leds(void);

#endif // LED_F4_H_INCLUDED
