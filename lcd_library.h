#ifndef __lcd_library_h
#define __lcd_library_h

#include <iom128v.h>
#include "delay.h"

#define LCD_WDATA   PORTA
#define LCD_WINST   PORTA
#define LCD_CTRL    PORTG
#define LCD_EN      0
#define LCD_RW      1
#define LCD_RS      2

#define On  1
#define Off 0

#define sbi(x, y)   (x |= (1<<y))
#define cbi(x, y)   (x &= ~(1<<y))

void LCD_delay(Byte ms);
void PortInit(void);
void LCD_Data(Byte ch);
void LCD_Comm(Byte ch);
void LCD_CHAR(Byte c);
void LCD_STR(Byte *str);
void LCD_pos(unsigned char row, unsigned char col);
void LCD_Clear(void);
void LCD_Init(void);
void Cursor_Home(void);

#endif
