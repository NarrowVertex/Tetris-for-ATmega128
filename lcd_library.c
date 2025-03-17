#include "lcd_library.h"

void LCD_delay(Byte ms)
{
    delay_ms(ms);
}

void PortInit(void)
{
    DDRA = 0xFF;
    DDRG = 0x0F;
}

void LCD_Data(Byte ch)
{
    LCD_CTRL |= (1 << LCD_RS);
    LCD_CTRL &= ~(1 << LCD_RW);
    LCD_CTRL |= (1 << LCD_EN);
    delay_us(50);
    LCD_WDATA = ch;
    delay_us(50);
    LCD_CTRL &= ~(1 << LCD_EN);
}

void LCD_Comm(Byte ch)
{
    LCD_CTRL &= ~(1 << LCD_RS);
    LCD_CTRL &= ~(1 << LCD_RW);
    LCD_CTRL |= (1 << LCD_EN);
    delay_us(50);
    LCD_WINST = ch;
    delay_us(50);
    LCD_CTRL &= ~(1 << LCD_EN);
}

void LCD_CHAR(Byte c)
{
    LCD_Data(c);
    delay_ms(1);
}

void LCD_STR(Byte *str)
{
    while(*str != 0)
    {
        LCD_CHAR(*str);
        str++;
    }
}

void LCD_pos(unsigned char row, unsigned char col)
{
    LCD_Comm(0x80 | (row * 0x40 +col));
}

void LCD_Clear(void)
{
    LCD_Comm(0x01);
    LCD_delay(2);
}

void LCD_Init(void)
{
    LCD_Comm(0x38);
    LCD_delay(2);
    LCD_Comm(0x38);
    LCD_delay(2);
    LCD_Comm(0x38);
    LCD_delay(2);
    LCD_Comm(0x0e);
    LCD_delay(2);
    LCD_Comm(0x06);
    LCD_delay(2);
    LCD_Clear();
}
