#include <iom128v.h>
#include "delay.h"
#include "lcd_library.h"
#include "tetris.h"

int time_count = 0;
int ADCW_count = 0;
unsigned int ADCW = 0;

unsigned int score = 0;
unsigned int received_score = 0;
unsigned int received_count;
unsigned char rxdata = 0;
unsigned char isReceiving = 0;

void print_map(void);
void print_score(void);
void send_score(void);

#pragma interrupt_handler timer0_ovf_isr:iv_TIM0_OVF
void timer0_ovf_isr(void)
{
    if(time_count >= 10000)
    {
        // PORTB ^= PORTB;     // TEST
        if(game_tick())
        {
            score += 100;
            send_score();
        }
        print_map();
        time_count = 0;
    }
    time_count++;
    TCNT0 = 158;
}

#pragma interrupt_handler timer2_ovf_isr:iv_TIM2_OVF
void timer2_ovf_isr(void)
{
    if(ADCW_count >= 10)
    {
        while( !(ADCSRA & (1<<ADIF))); // Polling 방식
        ADCW = ADCL + ADCH*256; // AC 변환 데이터 읽음

        // 1<=ADCW<=12
        pos_block(ADCW / 85);
        print_map();

        ADCW_count = 0;
    }
    ADCW_count++;
    TCNT2 = 158;
}

void init_timer(void)
{
    TCCR0 = (1<<CS01);
    TCCR2 = (1<<CS21);
    TIMSK |= (1<<TOIE0) | (1<<TOIE2);
    TCNT0 = 158;
    TCNT2 = 158;
}


#pragma interrupt_handler usart0_receive:iv_USART0_RXC
void usart0_receive(void)
{
    rxdata = UDR0;
    if(rxdata == 10)
    {
        received_score = 0;
        received_count = 0;
    }
    else if(rxdata == 20)
    {
        print_score();
    }
    else
    {
        received_score |= rxdata << received_count;
        received_count++;
    }
}

void init_USART0(void)
{
    UBRR0H = 0x00;
    UBRR0L = 103;
    UCSR0A = 0x00;
    UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
    UCSR0C = (1<<UPM01)|(1<<UCSZ01)|(1<<UCSZ00);
}

void putch_USART0(char data)
{
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = data;
}

void send_score(void)
{
    putch_USART0(10);

    int temp = score;
    while(temp > 0)
    {
        putch_USART0(temp & (0x01));
        temp >>= 1;
    }

    putch_USART0(20);
}

// ccw rotate
#pragma interrupt_handler ext_int0_isr:iv_EXT_INT0
void ext_int0_isr(void)
{
    ccw_block();
    print_map();
}

// cw rotate
#pragma interrupt_handler ext_int1_isr:iv_EXT_INT1
void ext_int1_isr(void)
{
    cw_block();
    print_map();
}

#pragma interrupt_handler ext_int2_isr:iv_EXT_INT2
void ext_int2_isr(void)
{
    game_tick();
    print_map();
}

void init_AD(void)
{
    DDRF = 0x00;
    ADMUX = 0x00;
    ADCSRA |= (1<<ADEN) | (1<<ADSC) | (1<<ADFR) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
}

void print_map(void)
{

}

void print_score(void)
{

}


void init_button(void)
{
    EIMSK = 0x07;       // ext interrupt: 0, 1, 2
    EICRA = 1<<ISC01;
    SREG |= 0x80;
}

void main(void)
{
    DDRB = 0xFF;
    PORTB = 0xFF;
    SREG |= 0x80;

    init_timer();
    init_button();
    init_AD();
    init_USART0();

    init_game();

    LCD_Init();
    LCD_Clear();

    while(1) { }
} // end of main loop
