#include "test.h"

void init_timer(void)
{
    TCCR0 = (1<<CS01);
    TCCR2 = (1<<CS21);
    TIMSK |= (1<<TOIE0) | (1<<TOIE2);
    TCNT0 = 158;
    TCNT2 = 158;
}

void init_USART0(void)
{
    UBRR0H = 0x00;
    UBRR0L = 103;
    UCSR0A = 0x00;
    UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
    UCSR0C = (1<<UPM01)|(1<<UCSZ01)|(1<<UCSZ00);
}

void init_AD(void)
{
    DDRF = 0x00;
    ADMUX = 0x00;
    ADCSRA |= (1<<ADEN) | (1<<ADSC) | (1<<ADFR) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
}

void init_button(void)
{
    EIMSK = 0x07;       // ext interrupt: 0, 1, 2
    EICRA = 1<<ISC01;
    SREG |= 0x80;
}
