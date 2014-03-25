#include "LCD_HD44780.h"


#define LCD_8DATA_BIT


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// function definitions
void LCD_wait(E_waitType wType){
    int times=0;
    switch(wType){
        case(sendCmd): times = 100; break;  // Wait 5ms for command writes
        case(sendData): times = 50; break;     // and 200us for data writes.
        case(setEnable): times = 50; break;  // Wait At Least 450 ns!!!
        default: times = 10;
    }

    for (;times>=0; times--)
        for (int i = 0; i < 10; i++)
            __asm__("nop");
}

// indexed from zero
uint8_t LCD_gotoxy(lcd_device_t *dev, uint8_t x, uint8_t y){
    if(x>HD44780_P_XMAX-1) return('x');
    switch(y){
        case(0):
            LCD_writeCmd(dev, LCD_C_CUR_ADDRESS_L1+x); break;
        case(1):
            LCD_writeCmd(dev, LCD_C_CUR_ADDRESS_L2+x); break;
        default:
            return('y');
    }
    return(0);
}

void LCD_writeChar(lcd_device_t *dev,uint8_t data){
    LCD_write(dev,data,sendData);
}
void LCD_writeCmd(lcd_device_t *dev,uint8_t cmd_data){
    LCD_write(dev,cmd_data,sendCmd);
}

void LCD_write(lcd_device_t *dev, uint8_t data, E_waitType wType){
    gpio_clear(dev->cmd_port, dev->cmdRW); // cmd: writing
    if(wType == sendCmd){
        gpio_clear(dev->cmd_port, dev->cmdRS); // cmd: sending cmd
        LCD_wait(setEnable);
        LCD_writeByte(dev,data, sendCmd);
    }
    else {
        gpio_set(dev->cmd_port, dev->cmdRS); // cmd: sending data
        LCD_wait(setEnable);
        LCD_writeByte(dev,data, sendData);
    }
}

#ifdef LCD_4DATA_BIT

void LCD_writePort(uint8_t data){
    uint16_t LCD_port = 0x0000;
    if( data & GPIO4) LCD_port |= LCD_D4;
    if( data & GPIO5) LCD_port |= LCD_D5;
    if( data & GPIO6) LCD_port |= LCD_D6;
    if( data & GPIO7) LCD_port |= LCD_D7;

    gpio_port_write(PLCD_DATA,LCD_port);
}

void LCD_writeByte(uint8_t data, E_waitType wType){
    gpio_clear(PLCD_CMD,LCD_EN);
    // write HIGH NIBBLE of the data/command on D7-4
    uint8_t nibble = data & 0xF0;
    LCD_writePort( nibble );
        gpio_set(PLCD_CMD, LCD_EN);
            LCD_wait(setEnable);
        gpio_clear(PLCD_CMD,LCD_EN);
    LCD_wait(wType);

    // write the LOW NIBBLE of the data/command on D7-4
    nibble = (data<<4) & 0xF0;
    LCD_writePort(nibble);
        gpio_set(PLCD_CMD, LCD_EN);
            LCD_wait(setEnable);
        gpio_clear(PLCD_CMD,LCD_EN);
    LCD_wait(wType);
}
#endif // LCD_4DATA_BIT
#ifdef LCD_8DATA_BIT

void LCD_writePort(lcd_device_t *dev, uint8_t data){
    uint16_t LCD_port = 0x0000;
    uint8_t i=0;
    for(;i<8;i++)
        if( data & ((1LU)<<i))
            LCD_port |= dev->data_pins[i];
    gpio_port_write(dev->data_port, LCD_port); // expose data/cmd on data bus
}

void LCD_writeByte(lcd_device_t *dev, uint8_t data, E_waitType wType){
    gpio_clear(dev->cmd_port,dev->cmdEN);
    LCD_writePort( dev, data );
        gpio_set(dev->cmd_port, dev->cmdEN);
        __asm__("nop");__asm__("nop");__asm__("nop");
        gpio_clear(dev->cmd_port, dev->cmdEN);
    LCD_wait(wType);
}
#endif // LCD_8DATA_BIT


void asmwait(int times){ for(;times>=0;times--) __asm__("nop"); }

void INIT_LCD(void)
{
    /*
    // enable the clocks
    rcc_periph_clock_enable(RCC_LCD_DATA);
    rcc_periph_clock_enable(RCC_LCD_CMD);

    // init pins
	gpio_mode_setup(PLCD_DATA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_DATA_ALL);
	gpio_mode_setup(PLCD_CMD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_CMD_ALL);
	// pin value
	gpio_clear(PLCD_DATA, LCD_DATA_ALL); // Data bus low
    gpio_clear(PLCD_CMD, LCD_RS|LCD_RW); // Control bus low
    gpio_clear(PLCD_CMD,LCD_EN);        // Enable display
    //----------------------------------------------------------
*/
/*
        //if(b==0) LCD_pos0();
        //LCD_writeCmd(0x38); // 2 line - no backlight
        //LCD_writeCmd(0x30); // 1 line + backlight
        //LCD_writeCmd(0x38); // Function set (8-bit interface, 2 lines, 5*7 Pixels)

        LCD_writeCmd(0x30); asmwait(10);
        LCD_writeCmd(0x30); asmwait(10);
        LCD_writeCmd(0x30); asmwait(10);

        //LCD_writeCmd(0x38); // Function set (8-bit interface, 2 lines, 5*7 Pixels)
        LCD_writeCmd(0x30); // Function set (8a-bit interface, 2 lines, 5*7 Pixels) + back
        LCD_writeCmd(0x06); // entry mode - normal

        LCD_writeCmd(0x0F); // Turn on visible blinking-block cursor
        LCD_writeCmd(LCD_C_CLR0); // Home (move cursor to top/left character position)
*/


    //char ch = 's';

/*
    //LCD_writeCmd(0x28); // 2 line 5x7 dots
    // LCD Commands = http://www.geocities.com/dinceraydin/lcd/commands.htm
    LCD_writeCmd(0x38); // Function set (8-bit interface, 2 lines, 5*7 Pixels)
    LCD_writeCmd(0x0F); // Turn on visible blinking-block cursor

    LCD_writeCmd(0x06); // Display Shift :OFF, Increment Address Counter
    */
/*
Enable Display/Cursor
 0000 1DCB
 D - Turn Display On(1)/Off(0)
 C - Turn Cursor On(1)/Off(0)
 B - Cursor Blink On(1)/Off(0)
 // 0000 1111
 //  Display on/Cursor on/Blinking on
 */
    //LCD_writeCmd(0x0F);
    //LCD_writeCmd(0xF0); // no
    //
    //LCD_pos0();
}

  /*
    _lcd_delay(50); // Wait after power on

    gpio_port_write()
    LCD_PORT->ODR |= (1<<DB4) | (1<<DB5);
    _lcd_strob();
    _lcd_delay(5);
    _lcd_strob();
    _lcd_delay(1);
    _lcd_strob();
    _lcd_delay(1);
    // 4 bit Init
    LCD_PORT->ODR &= ~(1<<DB4);
    _lcd_strob();
    _lcd_delay(5);

    // LCD config
    _lcd_write(0x28, CMD); // 2 line 5x7 dots
    _lcd_write(0x0C, CMD); // Display on/Cursor off/Blinking off
    _lcd_write(0x06, CMD); // Set cursor move direction Increase
*/
/*
//-------------------------------------------------------------
//--- Set cursor to position ----------------------------------
void LCD_GotoXY(uint8_t x, uint8_t y)
{
    if (y == 0)
        _lcd_write(80 + x, 0);
    else
        _lcd_write(0xC0 + x, 0);
}
//-------------------------------------------------------------
//--- Send character to LCD -----------------------------------
void LCD_PrintChar(char c)
{
    _lcd_write(c, DATA);
}
//-------------------------------------------------------------
//--- Send text to LCD ----------------------------------------
void LCD_PrintString(char *s)
{
    while(*s)
    {
        _lcd_write(*s++, 1);
    }
}
//--------------------------------------------------------------
//--- Write to LCD ---------------------------------------------
void _lcd_write(uint8_t data, uint8_t rs_bit)
{
    _lcd_delay(10);
    _lcd_write_nibble(data>>4, rs_bit);
    _lcd_write_nibble(data, rs_bit);
}
//--------------------------------------------------------------
//--- LCD Clear ------------------------------------------------
void LCD_Clear(void)
{
    _lcd_write(0x01, CMD); // Display clear
    _lcd_write(0x02, CMD); // Return home
}
//-------------------------------------------------------------
//--- Write nibble --------------------------------------------
void _lcd_write_nibble(uint8_t nib, uint8_t rs_bit)
{
    unsigned char temp;
    temp = (nib<<4) & 0xF0;
    LCD_PORT->ODR = temp;

    if(rs_bit == 0)
    {
        gpio_clear(PLCD,0xFF); // clear port
    }
    else
    {
        LCD_PORT->BSRR = RS_ON; //set
    }

    _lcd_strob();
}
//--------------------------------------------------------------
//--- LCD E strob impuls ---------------------------------------
void _lcd_strob(void)
{
    LCD_PORT->BSRR = E_ON;
    __NOP();
    __NOP();
    __NOP();
    LCD_PORT->BSRR = E_OFF;
}
//-------------------------------------------------------------
//--- LCD Delay -----------------------------------------------
void _lcd_delay(uint32_t delay)
{
    delay = delay*1000;
        for(; delay != 0; delay--)
        {
            __NOP();
        }
}
*/
