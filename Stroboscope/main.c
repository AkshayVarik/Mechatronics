
/*
 * Stroboscope.c
 *
 * Created: 10/4/2015 1:56:16 AM
 * Author : Akshay
 */ 

#include <avr/io.h>
#include "m_general.h"
#include "m_usb.h"
#include "m_rf.h"
#include "m_bus.h"


/*void Timer1Function(void);*/

 float sinetable[50]=
{
	25,28,31,34,37,40,42,44,
	46,48,49,50,50,50,50,49,
	48,46,44,42,40,37,34,31,
	28,25,22,19,16,13,10,8,
	6,4,2,1,0,0,0,0,
	1,2,4,6,8,10,13,16,
	19,22,25
};

volatile int i=0;
float X;

int main(void)
{
m_clockdivide(3);	
	

//OCR1B=0;

set(DDRB,6); // sets the output pin at B6


m_red(ON); // indicator that this code has begun
OCR1A=1000;
sei(); // enable global interrupts

set(TCCR1B,WGM13); // Single-Slope: Timer UP to a value, reset to 0x0000 (set/reset PWM): MODE 15 UP to OCR1A (8-bit), PWM mode
set(TCCR1B,WGM12); // ^
set(TCCR1A,WGM11); // ^
set(TCCR1A,WGM10); // ^

set(TCCR1A,COM1B1); // clear at OCR1B, set at rollover
clear(TCCR1A,COM1B0); //^

set(TIMSK1,TOIE1); //interrupt at OCR1A

clear(TCCR1B, CS12); // pre-scaler /8
set(TCCR1B, CS11); // ^
clear(TCCR1B, CS10); // ^


while(1)
{

}

}

// void Timer1Function(void)
// {
// 
// 
// 
//  // indicator that this code has been executed
// }


ISR(TIMER1_OVF_vect)
{
	//m_green(ON);
	X=(OCR1A*sinetable[i])/50;
	OCR1B=int(X);
	
	i++;
	
	if(i>50)
	{
		i=0;
	}

}

