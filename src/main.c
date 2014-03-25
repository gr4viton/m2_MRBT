
#include "main.h"
#define NOT_IMPLEMENTED_YET 0
#define IS_BUGGY 0
#define NOT_IMPORTANT 0
/*
TODO:
[] dodìlat seek na lcd
[] vytvoøit mu projekt na gitu
[] dat ho jako subprojekt
[] prejmenovat lcdHD... na device_LCD
[] udìlat jmennou konvenci

*/
#define LCD_DBUFSZ 1024
#define RBUFSZ 1024
#define TBUFSZ 1024
static uint8_t rbuf[RBUFSZ];
static uint8_t tbuf[TBUFSZ];
static uint8_t lcd_dbuf[LCD_DBUFSZ];

FILE *us2;
FILE *lcd;
void wwait(void);
void twait(int times);

extern lcd_device_t lcds[];

void wwait(void){twait(100);}

void twait(int times){
    for (;times>=0; times--)
        for (int i = 0; i < 10000; i++)
            __asm__("nop");
}
int main(void)
{
    us2 = fopenserial(3, 9600, tbuf, TBUFSZ, rbuf, RBUFSZ); // uarts[3] = UART4 = tC10,rC11
    //lcd = fopenlcd(1, 9600, tbuf,1024,rbuf,1024);

    lcd = fopenLCD(0,
                   0x38,//0x30, //LCD_C_8BIT_2L_5x7_LIGHT, //LCD_C_8BIT_1L_5x7_LIGHT,//
                   LCD_C_ENTRY_RIGHT_CMOVE,
                   LCD_C_CUR_VIS_BLINK,
                   lcd_dbuf, LCD_DBUFSZ);

    //ultra_sensor_t* ultra1 = 0;
    uint8_t i_ultra = 0;

    INIT_leds();
    INIT_ultra(i_ultra ,1);

    //INIT_LCD();

    //LCD_writeCmd(dev,LCD_C_CLR0);

    uint8_t b = (uint8_t)'A';

/*
    LCD_Init();
    LCD_Clear();

    LCD_PrintString("> Hello  STM32 <");
    LCD_GotoXY(0,1);
    LCD_PrintString("----------------");
*/
    //INIT_LCD();

    float pi = 3.1415926535;
    //char a = 32;
    uint8_t last = 0;
    uint8_t x,y;
    x=y=0;
	while (1) {
        gpio_toggle(PLED,LED0);
        /*
        LCD_gotoxy(&(lcds[0]),0,0);
        */
        fprintf(lcd, "dist[cm]=%f", ULTRA_getDist(i_ultra));
        fprintf(lcd, "ABCDEFGH!");
        wwait();
        //fprintf(lcd, "a", pi,b);
        //fputc(a++,lcd);
/*
        if(last == 'x') x=0;
        if(last == 'y') y=0;
        last = LCD_gotoxy(&(lcds[0]),x++,y);
        fputc('R',lcd);
        */
        /*
        while(b++<5){
            //fseek(lcd,b*4,SEEK_SET);
            LCD_gotoxy(&(lcds[0]),x,y)
            fputc('x',lcd);
            gpio_toggle(PLED,LED0);
            //wwait();
        }*/
        //fseek(lcd,0,SEEK_SET);
        last++;
        //fprintf(us2,
	}

	return 0;
}

