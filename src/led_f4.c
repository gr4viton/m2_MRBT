#include "led_f4.h"

void INIT_leds(void)
{
	rcc_periph_clock_enable(RCC_PLED);
	gpio_mode_setup(PLED, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_ALL);
	gpio_clear(PLED, LED_ALL);
}
