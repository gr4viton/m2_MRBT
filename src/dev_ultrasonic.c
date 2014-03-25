#include "dev_ultrasonic.h"

typedef struct
{
    // VOLATILE !!!!!!!!!!!
    uint32_t clk;
    uint8_t irq;
    uint32_t txport;
    uint32_t rxport;
    uint16_t txpin;
    uint16_t rxpin;

    double dist;
    double proportion;
    uint32_t nOverflows;
    uint32_t ticksStart;
    uint32_t ticksEnd;
    uint32_t nTicks;

} ultra_sensor_t;

ultra_sensor_t ultras[2] = {
    {.clk=RCC_GPIOD, .txport=GPIOD, .rxport=GPIOD, .txpin=GPIO0, .rxpin=GPIO1 }
    ,{.clk=RCC_GPIOB, .txport=GPIOB, .rxport=GPIOB, .txpin=GPIO0, .rxpin=GPIO1 }
};

ultra_sensor_t* INIT_ultra(uint8_t index, double prop)
{
    ultra_sensor_t* dev = &ultras[index];

    dev->proportion = prop;

    // get RCC from port and enable
    //rcc_periph_clock_enable(GET_rcc_from_port(dev->txport))); //????
	rcc_periph_clock_enable(dev->clk);

	gpio_mode_setup(dev->txport, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, dev->txpin);
	gpio_mode_setup(dev->rxport, GPIO_MODE_INPUT, GPIO_PUPD_NONE, dev->rxpin);
	gpio_clear(dev->txport, dev->txpin);

	// setup timer to count with overflow interrupt (for detection)
	// to be able to count from x us to y ms

    // start counter
}

double ULTRA_getDist(uint8_t index){
    return ultras[index].dist;
}
void ULTRA_signalSend(ultra_sensor_t *dev)
{
    gpio_set(dev->txport, dev->txpin);
    //dev->ticksStart = timer5;
    // wait
    twait(10);
    gpio_clear(dev->txport, dev->txpin);
}

void ULTRA_signalAcquired(ultra_sensor_t *dev){
    // proportion
    dev->nTicks = (dev->ticksEnd - dev->ticksStart); //+ (dev->nOverflow * dev->..;
    dev->dist = dev->proportion * ( dev->nTicks / dev->clk );
}

void gpiod_isr(void)
{
    ultra_sensor_t *dev;
    //switch(INTERRUPTED_PIN){
    //case(ultras[0]):
        dev=&ultras[0];
    //}

    //!!!!
    //dev->ticksEnd = timer5a;
    ULTRA_signalAcquired(dev);
}

void timer3a_isr(){
    // find out which counting is active and increment overflow
    //dev->overflow++
}
