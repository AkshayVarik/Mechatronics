/*
 * Golfer.c
 *
 * Created: 10/23/2015 6:50:14 PM
 * Author : Akshay
 */ 

#include <avr/io.h> // Header files
#include "m_general.h" // ^
#include <avr/interrupt.h> // ^
#include "m_bus.h" // ^
#include "m_rf.h" // ^

int i = 0; // Sinetable index count variable
float sound_time = 0; // Initialize the variable for which the sound has to be played
int deci_secs = 0; // Variable that will count till the time for which the sound has to be played
int frequency = 0; // Frequency value that is sent by the signal at which the sound has to be generated
volatile int j = 0; // Variable that will indicate the communication signal received. Takes value 1 when so

int main(void)
{
    m_clockdivide(0); // System clock 16 MHz
     
    clear(TCCR1B,CS12); // Prescaler /1
    clear(TCCR1B,CS11);	// ^
    set(TCCR1B,CS10); // ^
    
    set(TCCR1B,WGM13); // Time mode (15) UP to OCR1A, PWM Mode, Single Slope, reset to Ox0000
    set(TCCR1B,WGM12); // ^
    set(TCCR1A,WGM11); // ^
    set(TCCR1A,WGM10); // ^
    
    set(TCCR1A,COM1B1); // Clear at OCR1B, set at rollover
    clear(TCCR1A,COM1B0); // ^
    
    set(TIMSK1, OCIE1A); // Sets an interrupt when Timer 1 overflows
    
    OCR1A = 1000; // Initializing the value
    OCR1B = 0; // Initializing the value
    
    sei(); // Enables global interrupt
    
    while(1)
	{
        if (j == 1) // If signal is received
		{
            m_green(ON);
            
            while (deci_secs < sound_time) // The count is less than the time for which the music has to be played
			{
                m_wait(10); // Wait for 10 centisecond/millisecond?
                deci_secs++; // Increment the count
            }
            m_green(OFF);
            OCR1A = 0; // Reset the OCR1A to 0. It will take a value next time there is a signal
            deci_secs = 0; // Reset the count to 0. It will take a value next time there is a signal
            j = 0; // ^
        }
    }
}

ISR(TIMER1_COMPA_vect)
{
	m_red(ON);
    OCR1B = ((sinetable[i]*OCR1A) / 50); // Generates the sine wave. OCR1B takes values between 0 and OCR1A as generated from the SineTable
    set(DDRB,6); // Output pin
    i++; // Increment
	if(i>26) // In case the value surpasses the number of values in the the sinetable reset the index count variable to 0
	{
		i=0; // Variable set to 0
	}
}


ISR(INT2_vect) 
{
    
    if(m_rf_read(buffer,PACKET_LENGTH))
    {
			frequency = buffer[0] + 256 * buffer[1]; // 1st two elements of the packet represent the frequency       
            OCR1A = (16050-(15*frequency))/7; // Converts the frequency in deciHertz to OCR1A value based on the observation on the scope that the OCR1A value was 900 for 650 Hz and 1350 for 440 Hz
            sound_time = (int) buffer[2]; // Time for which the sound has to be played
            j = 1; // Becomes 1 every time it receives a signal
    }
}

