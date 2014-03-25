#ifndef LCD_HD44780_H_INCLUDED
#define LCD_HD44780_H_INCLUDED

/*
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <sys/types.h>
*/

#include <stdio.h>
//#include <sys/unistd.h>

#include <libopencm3/stm32/gpio.h>

//#include <sys/stat.h>
//#include <sys/times.h>
//#include <sys/unistd.h>


#include "primitives/ringbuf.h"
#include "main.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// macro definitions

/*
// not used anymore whilst using fopencmd and lcd_device_t
#define PLCD_DATA GPIOE
#define PLCD_CMD GPIOD

#define RCC_LCD_DATA RCC_GPIOE
#define RCC_LCD_CMD RCC_GPIOD

// "R/S" Register Select ( 1 for Data Write, 0 for Command Write)
#define LCD_RS GPIO11

// "R/W" Read/Write (1 for Read, 0 for Write)
#define LCD_RW GPIO13

// "EN" Enable line (Pulsing high latches a command or data _|"|_ )
#define LCD_EN GPIO15

//#define LCD_4DATA_BIT
#define LCD_8DATA_BIT

// control pins
#define LCD_CMD_ALL  (LCD_RW|LCD_RS|LCD_EN)

//Data Pins (D0-D7) D0 is LSB, in 4-bit mode only D4-D7 are used
#define LCD_D4 GPIO7
#define LCD_D5 GPIO9
#define LCD_D6 GPIO11
#define LCD_D7 GPIO13
#define LCD_DATA_ALL       (LCD_D4|LCD_D5|LCD_D6|LCD_D7)

#ifdef LCD_8DATA_BIT
    #define LCD_D0 GPIO8
    #define LCD_D1 GPIO10
    #define LCD_D2 GPIO12
    #define LCD_D3 GPIO14
    #undef LCD_DATA_ALL
    #define LCD_DATA_ALL (LCD_D0|LCD_D1|LCD_D2|LCD_D3)|(LCD_D4|LCD_D5|LCD_D6|LCD_D7)
#endif // LCD_8DATA_BIT

*/

// HD44780 physical parameters
#define HD44780_P_XMAX      16
#define HD44780_P_YMAX      2

// CONTROL COMMANDS
#define LCD_C_CLR0                  0x01
#define LCD_C_POS0                  0x02

#define LCD_C_ENTRY_LEFT_CMOVE      0x04
#define LCD_C_ENTRY_RIGHT_CSTAY     0x05
#define LCD_C_ENTRY_RIGHT_CMOVE     0x06
#define LCD_C_ENTRY_LEFT_CSTAY      0x07

#define LCD_C_CUR_MOV_LEFT          0x10
#define LCD_C_CUR_MOV_RIGHT         0x14
#define LCD_C_CUR_VIS_NO            0x0C
#define LCD_C_CUR_VIS_STATIC        0x0E
#define LCD_C_CUR_VIS_BLINK         0x0F

#define LCD_C_CUR_ADDRESS_L1        0x80
#define LCD_C_CUR_ADDRESS_L2        0xC0
#define LCD_C_CHAR_GENERATOR_ADD    0x40


/*
0 0 1 DL | L F * *
Set Interface Length
 DL - Set Data Interface Length 8(1)/4(0)
 L - Number of Display Lines 1(0)/2(1)
 F - Character Font 5x10(1)/5x7(0)

0 0 1 DL | N F * * < datasheet
0 0 1 DL | LI F * * ??
          h  DL  N   F     LI
0010 0000 0  8   1   5x7   1
0010 0001 1  8   1   5x7   1
0010 0010 2  8   1   5x7   1
0010 0011 3  8   1   5x7   1
0010 0100 4  8   1   5x10   1
0010 0101 5  8   1   5x10   1
0010 0110 6  8   1   5x10   1
0010 0111 7  8   1   5x10   1
0010 1000 8  8   2   5x7    0
0010 1001 9  8   2   5x7    0
0010 1010 A  8   2   5x7    0
0010 1011 B  8   2   5x7    0
0010 1100 C  8   1   5x10   0
0010 1101 D  8   1   5x10   0
0010 1110 E  8   1   5x10   0
0010 1111 F  8   1   5x10   0

 */
#define LCD_C_INIT                  0x30

#define LCD_C_8BIT_1L_5x7_LIGHT     0x30
#define LCD_C_8BIT_1L_5x10_LIGHT    0x34
#define LCD_C_8BIT_1L_5x7           0x38
#define LCD_C_8BIT_1L_5x10          0x3C

#define LCD_C_8BIT_2L_5x7_LIGHT     0x3A
#define LCD_C_8BIT_2L_DARK         0x38

#define LCD_C_DISP_BLANK        0x08
#define LCD_C_SCROLL_LEFT       0x18
#define LCD_C_SCROLL_RIGHT      0x1E


#include "dev_LCD_HD44780.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// type prototypes
typedef enum _E_waitType{
    sendCmd, // after enable clear when sending command
    sendData, // after enable clear when sending data
    setEnable // after enable set
    }E_waitType;


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// function prototypes
void LCD_wait(E_waitType wType);

void LCD_write(lcd_device_t *dev, uint8_t data, E_waitType wType);
void LCD_writeChar(lcd_device_t *dev, uint8_t data);
void LCD_writeCmd(lcd_device_t *dev, uint8_t cmd_data);

void LCD_writePort(lcd_device_t *dev, uint8_t data);
void LCD_writeByte(lcd_device_t *dev, uint8_t data, E_waitType wType);


uint8_t LCD_gotoxy(lcd_device_t *dev, uint8_t x, uint8_t y);

void asmwait(int times);

void INIT_LCD(void);
/*
void LCD_gotoXY(uint8_t x, uint8_t y);
void LCD_PrintChar(char c);
void LCD_PrintString(char *s);
void LCD_Clear(void);

void _lcd_write(uint8_t data, uint8_t rs_bit);
void _lcd_write_nibble(uint8_t nib, uint8_t rs_bit);
void _lcd_strob(void);
void _lcd_delay(uint32_t time);
*/





//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

 /*
R/S	R/W	D7	D6	D5	D4	D3	D2	D1	D0	Instruction/Description
________________________________________________________________________________________
0	0	0	0	0	0	0	0	0	1	Clear Display and Home the Cursor
0	0	0	0	0	0	0	0	1	*	Return Cursor and LCD to Home Position
0	0	0	0	0	0	0	1	ID	S	Set Cursor Move Direction
0	0	0	0	0	0	1	D	C	B	Enable Display/Cursor
0	0	0	0	0	1	SC	RL	*	*	Move Cursor/Shift Display
0	0	0	0	1	DL	N	F	*	*	Set Interface Length
0	0	0	1	A	A	A	A	A	A	Move Cursor into CGRAM
0	0	1	A	A	A	A	A	A	A	Move Cursor to Display
0	1	BF	*	*	*	*	*	*	*	Poll the "Busy Flag"
1	0	D	D	D	D	D	D	D	D	Write a Character to the Display at the Current Cursor Position
1	1	D	D	D	D	D	D	D	D	Read the Character on the Display at the Current Cursor Position

"*" - Not Used/Ignored. This bit can be either "1" or "0"
Set Cursor Move Direction:
 ID - Increment the Cursor After Each Byte Written to Display if Set
 S - Shift Display when Byte Written to Display

Enable Display/Cursor
 D - Turn Display On(1)/Off(0)
 C - Turn Cursor On(1)/Off(0)
 B - Cursor Blink On(1)/Off(0)

Move Cursor/Shift Display
 SC - Display Shift On(1)/Off(0)
 RL - Direction of Shift Right(1)/Left(0)

Set Interface Length
 DL - Set Data Interface Length 8(1)/4(0)
 N - Number of Display Lines 1(0)/2(1)
 F - Character Font 5x10(1)/5x7(0)

Poll the "Busy Flag"
 BF - This bit is set while the LCD is processing

Move Cursor to CGRAM/Display
 A - Address

Read/Write ASCII to the Display
 D - Data
 */
#endif // LCD_HD44780_H_INCLUDED
