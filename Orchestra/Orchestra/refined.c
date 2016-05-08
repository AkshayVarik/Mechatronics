/* Name: main.c
 * Author: <SUPRAJ AKILETI>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include "m_general.h"
#include <avr/interrupt.h>
#include "m_bus.h"
#include "m_rf.h"

#define CHANNEL 1
#define RXADDRESS 0x23
#define PACKET_LENGTH 3

int x = 0x00;
float sound_time = 0;
int deci_secs = 0;
int buffer_01 = 0;
volatile int duration_flag = 0;

char buffer[PACKET_LENGTH] = {0,0,0};

float sine[64] = {
    0x20,0x23,0x26,0x29,0x2c,0x2f,0x32,0x34,
    0x37,0x39,0x3b,0x3c,0x3e,0x3f,0x3f,0x40,
    0x40,0x40,0x3f,0x3f,0x3e,0x3c,0x3b,0x39,
    0x37,0x34,0x32,0x2f,0x2c,0x29,0x26,0x23,
    0x20,0x1d,0x1a,0x17,0x14,0x11,0xe,0xc,
    0x9,0x7,0x5,0x4,0x2,0x1,0x1,0x0,
    0x0,0x0,0x1,0x1,0x2,0x4,0x5,0x7,
    0x9,0xc,0xe,0x11,0x14,0x17,0x1a,0x1d,
};


int main(void)
{
    m_clockdivide(0);
    
    m_bus_init();
    m_rf_open(CHANNEL,RXADDRESS,PACKET_LENGTH);
    
    clear(TCCR1B,CS12);
    clear(TCCR1B,CS11);
    set(TCCR1B,CS10);
    
    set(TCCR1B,WGM13);
    set(TCCR1B,WGM12);
    set(TCCR1A,WGM11);
    set(TCCR1A,WGM10);
    
    set(TCCR1A,COM1B1);
    set(TCCR1A,COM1B0);
    
    set(TIMSK1, OCIE1A);
    
    OCR1A = 0;
    OCR1B = 0;
    
    sei();
    
    while(1){
        if (duration_flag == 1) {
            m_green(ON);
            
            while (deci_secs < sound_time) {
                m_wait(10);
                deci_secs++;
            }
            m_green(OFF);
            OCR1A = 0;
            deci_secs = 0;
            duration_flag = 0;
        }
    }
}

ISR(TIMER1_COMPA_vect)
{
    OCR1B = (sine[x]*OCR1A) / 64;
    set(DDRB,6);
    x = (x + 1) % 64;
}


ISR(INT2_vect) {
    
    if(m_rf_read(buffer,PACKET_LENGTH))
    {
        buffer_01 = buffer[0] + 256 * buffer[1];
        
            OCR1A = 2275-1.2857*(buffer_01-1100);
            sound_time = (int) buffer[2];
            duration_flag = 1;
    }
}


// 110(2275)-180(1375)
//1.5K resistance
//1uf+470pf


