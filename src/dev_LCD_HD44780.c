#include "dev_lcd_HD44780.h"

#define UNUSED(x) (void)(x);

static inline size_t min(const size_t a,const size_t b)
{
    return a < b ? a : b;
}

static ssize_t _iord(void *_cookie, char *_buf,size_t _n);
static ssize_t _iowr(void *_cookie,const char *_buf, size_t _n);
static int _ioseek(void *_cookie, off_t *_off,int _whence);
static int _ioclose(void *_cookie);
//static void _isru(lcd_device_t *dev);
static void _txsignal(struct ringbuf *rb);

/*
static lcd_device_data_port_t lcd_device_data_predef[] = {
    { .D0= GPIO7,   .D1= GPIO9,   .D2= GPIO11,  .D3= GPIO13,
      .D4= GPIO8,   .D5= GPIO10,  .D6= GPIO12,  .D7= GPIO14 },
    { .D0= GPIO0,   .D1= GPIO1,   .D2= GPIO2,  .D3= GPIO3,
      .D4= GPIO4,   .D5= GPIO5,  .D6= GPIO6,  .D7= GPIO7 }
};*/

static uint16_t lcd_device_data_predef[][8] = {
    { GPIO8, GPIO10, GPIO12, GPIO14, GPIO7, GPIO9, GPIO11, GPIO13 },
    { GPIO0, GPIO1, GPIO2, GPIO3, GPIO4, GPIO5, GPIO6, GPIO7 }
};

// for debug reasons %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//static lcd_device_t lcds[] =
lcd_device_t lcds[] =
    {
        { .cmd_port=GPIOD, .cmdp_clk=RCC_GPIOD, .data_port=GPIOE,  .datap_clk=RCC_GPIOE },
        { .cmd_port=GPIOB, .cmdp_clk=RCC_GPIOB, .data_port=GPIOE,  .datap_clk=RCC_GPIOE }
    };
/*
void usart1_isr(void) { _isru(&uarts[0]); }
void usart2_isr(void) { _isru(&uarts[1]); }
void usart3_isr(void) { _isru(&uarts[2]); }
void uart4_isr(void)  { _isru(&uarts[3]); }
void uart5_isr(void)  { _isru(&uarts[4]); }
void usart6_isr(void) { _isru(&uarts[5]); }
*/

void LCD_setDataPins(uint8_t index, lcd_device_t *dev);

void LCD_setDataPins(uint8_t index, lcd_device_t *dev){
    uint16_t _data_pins_all  = 0x0000;
    uint8_t i = 0;
    for(;i<8;i++) {
        dev->data_pins[i] = lcd_device_data_predef[index][i];
        _data_pins_all |= dev->data_pins[i];
    }
}

uint16_t LCD_getMaskDataPins(lcd_device_t *dev);

uint16_t LCD_getMaskDataPins(lcd_device_t *dev){
    uint16_t _data_pins_all = 0x0000;
    uint8_t i = 0;
    for(;i<8;i++)
        _data_pins_all |= dev->data_pins[i];
    return(_data_pins_all);
}
/*
LCD_setDataPins(uint8_t[]){
    data
}*/



FILE *fopenLCD(uint8_t index, uint8_t functionSet, uint8_t entryMode, uint8_t cursorMode,\
               uint8_t *dbuf, size_t dbufsz)
{
    lcd_device_t *dev = &lcds[index];

    uint8_t i=0;
    // initialize ring buffers
    ringbuf_init(&(dev->data_ring),dbuf,dbufsz);
    //ringbuf_init(&(dev->rx_ring),rbuf,rbufsz);
    dev->data_ring.signal = _txsignal;


    // predef ports and clks
    dev->datap_clk = lcds[index].datap_clk;
    dev->data_port = lcds[index].data_port;
    dev->cmdp_clk = lcds[index].cmdp_clk;
    dev->cmd_port = lcds[index].cmd_port;



    // data pins
    LCD_setDataPins(index,dev);
    //dev->data_pins_all = LCD_getMaskDataPins(dev);
    // cmd pins
    dev->cmdRS = GPIO11;
    dev->cmdRW = GPIO13;
    dev->cmdEN = GPIO15;
    dev->cmd_pins_all = 0x0000 | (dev->cmdRS | dev->cmdRW | dev->cmdEN);

    dev->datap_clk = RCC_GPIOE;
    dev->data_port = GPIOE;
    dev->data_pins[0] = GPIO8;
    dev->data_pins[1] = GPIO10;
    dev->data_pins[2] = GPIO12;
    dev->data_pins[3] = GPIO14;
    dev->data_pins[4] = GPIO7;
    dev->data_pins[5] = GPIO9;
    dev->data_pins[6] = GPIO11;
    dev->data_pins[7] = GPIO13;
    for(;i<8;i++)
        dev->data_pins_all |= dev->data_pins[i];

    // enable the clocks
    rcc_periph_clock_enable(dev->cmdp_clk);
    rcc_periph_clock_enable(dev->datap_clk);

    // init pins
    gpio_mode_setup(dev->data_port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, dev->data_pins_all);
    gpio_mode_setup(dev->cmd_port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, dev->cmd_pins_all);

    // reset lcd pins
	gpio_clear(dev->data_port, dev->data_pins_all); // Data bus low
    gpio_clear(dev->cmd_port, dev->cmd_pins_all); // Control bus low, Enable display

    // init lcd
    LCD_writeCmd(dev,LCD_C_INIT);
    LCD_wait(sendCmd);
    LCD_writeCmd(dev,LCD_C_INIT);
    LCD_wait(sendData);
    LCD_writeCmd(dev,LCD_C_INIT);

    // set initial params
    LCD_writeCmd(dev,functionSet);
    LCD_writeCmd(dev,entryMode);
    LCD_writeCmd(dev,cursorMode);

    LCD_writeCmd(dev,LCD_C_CLR0);
/*
    // this is not initialization - these are settings

        LCD_writeCmd(0x30); asmwait(10);
        LCD_writeCmd(0x30); asmwait(10);
        LCD_writeCmd(0x30); asmwait(10);

        //LCD_writeCmd(0x38); // Function set (8-bit interface, 2 lines, 5*7 Pixels)
        LCD_writeCmd(0x30); // Function set (8a-bit interface, 2 lines, 5*7 Pixels) + back
        LCD_writeCmd(0x06); // entry mode - normal

        LCD_writeCmd(0x0F); // Turn on visible blinking-block cursor
        LCD_writeCmd(LCD_C_CLR0); // Home (move cursor to top/left character position)
*/

    // init irqs
	//usart_enable_rx_interrupt(dev->device);
	//nvic_enable_irq(dev->irq);

    // run the serail line
    //usart_enable(dev->device);

    // stdio stub
    cookie_io_functions_t stub = { _iord, _iowr, _ioseek, _ioclose };
    FILE *fp = fopencookie(dev,"rw+",stub);
    setvbuf(fp, NULL, _IONBF, 0);
    return fp;
}

/*
struct ringbuf *ser_txbuf(uint8_t index)
{
    return &uarts[index].tx_ring;
}
*/

/* Low level */

static ssize_t _iord(void *_cookie, char *_buf,size_t _n)
{
    // dont support reading now
    UNUSED(_cookie);
    UNUSED(_buf);
    UNUSED(_n);
    return 0;
}

static ssize_t _iowr(void *_cookie, const char *_buf, size_t _n)
{
    // tohle bude volat druhou funkci LCD_write s parametrem DATA (vždy)
    // pomoci fopencookie nebude možné nastavovat LCD -> pouze psát data
    // možná pøi initu se dá aj init všeho -> dvì možnosti, init bez zmìny
    // -> a druhá init s nastavením
    lcd_device_t *dev = (lcd_device_t *)_cookie;
    // prozatím vynechám buffer
    int written = 0;
    //int c = 0;
    uint8_t i = 0;
 //       LCD_writeChar(dev,_buf[i]);
    do {
        LCD_writeChar(dev,(uint8_t)_buf[i]);
        _n--;
        written++;
    } while(_n>0);//_buf[i]!='\0');

    return written;
/*
    do {
        //nvic_disable_irq(dev->irq);

        c = ringbuf_write_buffer_partial(&dev->tx_ring, _n, (const uint8_t *)_buf);

        //usart_enable_tx_interrupt(dev->device);
        //nvic_enable_irq(dev->irq);

        _n -= c;
        _buf += c;
        written += c;

        if (_n > 0)                // wait for the write some character succeeded
            while (ringbuf_write_available(&dev->tx_ring) < min(_n, dev->tx_ring.mask-1))
                {}; // generic idle
    } while (_n > 0);
    return written;
    */
}

static int _ioseek(void *_cookie, off_t *_off,int _whence)
{
    lcd_device_t *dev = (lcd_device_t *)_cookie;

    uint16_t pos = 0;
    switch(_whence){
        case(SEEK_SET): pos = 0; break;
        case(SEEK_CUR): /*GET_POS..*/; break;
        case(SEEK_END): pos = 0x20; break;
        default: break;
    }
    // add offset
    if(*_off<=20) pos = *_off + LCD_C_CUR_ADDRESS_L1;
    if(pos>20) pos = *_off + LCD_C_CUR_ADDRESS_L2;
    LCD_writeCmd(dev,pos);

    return(0);
    /*
    UNUSED(_whence);
    UNUSED(_off);
    UNUSED(_cookie);
    return EINVAL;*/
}

static int _ioclose(void *_cookie)
{
    UNUSED(_cookie);
    return EINVAL;
}

void usa_rxb(uint8_t ch);

/*
static void _isru(uart_device_t *dev)
{
    uint8_t ch;

    if (usart_get_interrupt_source(dev->device,USART_SR_TXE))
    {
        if (ringbuf_read_byte(&dev->tx_ring, &ch))
            usart_send(dev->device, ch);
        else
            usart_disable_tx_interrupt(dev->device);
    }

    if (usart_get_interrupt_source(dev->device,USART_SR_RXNE))
    {
        ch = (char)usart_recv(dev->device);

        //usa_rxb(ch);

        if (!ringbuf_write_byte(&dev->rx_ring, ch))
        {
            // data overrun occurs, current char will not be added
            dev->stats_rxoverruns++;
        }

        if (proto_rx64_extract(&dev->rx_ring) == PROTO_NOTMY)
            ringbuf_skip(&dev->rx_ring,1);

    }
}
*/

static void _txsignal(struct ringbuf *rb)
{
    UNUSED(rb);
    /*
    for (int i=0; i< 6; i++)
    {
        if (rb != &uarts[i].tx_ring)
            continue;

        usart_enable_tx_interrupt(uarts[i].device);
    }*/
}




