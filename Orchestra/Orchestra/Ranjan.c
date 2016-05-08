#include <avr/io.h>
#include "m_general.h"
#include "m_usb.h"
#include "m_bus.h"
#include "m_rf.h"


void init(void);
void init2(void);


int sinetable[51] = 
    {25,28,31,34,37,40,42,44,
46,48,49,50,50,50,50,49,
48,46,44,42,40,37,34,31,
28,25,22,19,16,13,10,8,
6,4,2,1,0,0,0,0,
1,2,4,6,8,10,13,16,
19,22,25};
     
 
volatile int i=0;
volatile int j=0;
volatile int frequency = 5500;
volatile int y;
int x;

char buffer[3] = {0,0,0};

void main(void)
{
    sei();
    OCR0A = 50;
    m_usb_init();
    m_clockdivide(0);
    init();
    init2();
    m_bus_init();
    m_rf_open(1,0x4E,3); 
 
     while(1)
    {
   
    m_usb_tx_string("    j value ");
    m_usb_tx_int(j);
    m_usb_tx_string("    Buffer value ");
    m_usb_tx_int(buffer[2]);
    m_usb_tx_string("\n");
         
    if(j == 30000)
    {
        j = 15000;
    }
    
    
            
}
}

ISR(TIMER0_OVF_vect)
{
    cli();
   
        if(j < ((buffer[2])*80)) 
   {
        j ++;
       set(DDRB,7);
       m_red(ON);
   }
    else
    {
        clear(DDRB,7);
        m_red(OFF);   
    }  
    sei();
}

ISR(TIMER1_OVF_vect)
{
    cli();
    OCR1C = (OCR1A*sinetable[i])/50;
    i++;
    if (i == 50)
    {
        i = 0;
    }  
    sei();
}

ISR(INT2_vect)
    {
    
    cli();
    m_rf_read(buffer,3);
    y = buffer[0] + (buffer[1]*256);
    
    frequency = y;
    x = frequency/100;
    OCR1A = 32400/x;
    j = 0;
    
    sei();

}






void init(void)
{     
    m_green(ON);
    

    
    set(TCCR1B, WGM13);
    set(TCCR1B, WGM12);
    set(TCCR1A, WGM11);
    set(TCCR1A, WGM10);
    
    OCR1A = 1000;
    OCR1C = 100;
    
    set(TCCR1A, COM1C1);
    clear(TCCR1A, COM1C0);
    
    set(TIMSK1, TOIE1);
    
    
    clear(TCCR1B, CS12);
    clear(TCCR1B, CS11);
    set(TCCR1B, CS10);
     
    m_green(OFF);   
    
}

void init2(void)
{
    
    clear(TCCR0B,WGM02);
    clear(TCCR0A,WGM01);
    clear(TCCR0A,WGM00);
    
    set(TIMSK0,TOIE0);
    
    clear(TCCR0B,CS02);
    set(TCCR0B,CS01);
    clear(TCCR0B,CS00);
    

}
