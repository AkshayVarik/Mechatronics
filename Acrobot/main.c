/* Name: main.c
 * Author: <SUPRAJ AKILETI>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include "m_general.h"
#include <avr/interrupt.h>
#include "m_bus.h"
#include "m_imu.h"
#include "m_usb.h"

#define dt 0.01

unsigned char accel_scale = 0;
unsigned char gyro_scale = 0;
int data[9] = {0};
unsigned char read;

volatile int a;
volatile int g;
volatile int gx;
volatile int gy;
volatile int gz;
volatile int ax;
volatile int ay;
volatile int az;
volatile int angle = 0;
volatile int a_home;
volatile int error;
volatile int kp = 0.42;
volatile int a_new=0;


int main(void)
{
    m_clockdivide(3); //2MHz
    
    //Timer
    clear(TCCR1B,CS12);
    set(TCCR1B,CS11);
    set(TCCR1B,CS10); //64 precale timer
    
    set(TCCR1B,WGM13);
    set(TCCR1B,WGM12);
    set(TCCR1A,WGM11);
    set(TCCR1A,WGM10);
    
    set(TCCR1A,COM1B1);
    clear(TCCR1A,COM1B0);
    
    m_usb_init();
    
    m_imu_init(0,0);
    
    
    //read=1;
    
    while(1){
        
        read = m_imu_raw(data);
        
        ax = data[0];
        ay = data[1];
        az = data[2];
        gx = data[3];
        gy = data[4];
        gz = data[5];
    
        
        a = pow((pow(ax,2) + pow(ay,2) + pow(az,2)),0.5);
        g = pow((pow(gx,2) + pow(gy,2) + pow(gz,2)),0.5);
        //angle = 0.6*(angle+(g*dt))+0.4*a;
        
        a_home = 390;
        error = a_home-a_new;
        a_new = ax+kp*error;
        
        if (error > 500) {
            m_red(OFF);
            clear(DDRD,5);
            clear(PORTD,5);

            set(DDRD,6);
            set(PORTD,6);
        } else {
            m_red(ON);
            clear(DDRD,6);
            clear(PORTD,6);
            
            set(DDRD,5);
            set(PORTD,5);
        }
        
        m_usb_tx_string("error\t");
        m_usb_tx_int(error);
        
        
//        m_usb_tx_string("\tax\t");
//        m_usb_tx_int(data[0]);
//        m_usb_tx_string("\tay\t");
//        m_usb_tx_int(data[1]);
//        m_usb_tx_string("\taz\t");
//        m_usb_tx_int(data[2]);
//        m_usb_tx_string("\tgx\t");
//        m_usb_tx_int(data[3]);
//        m_usb_tx_string("\tgy\t");
//        m_usb_tx_int(data[4]);
//        m_usb_tx_string("\tgz\t");
//        m_usb_tx_int(data[5]);
//        m_usb_tx_string("\ta\t");
//        m_usb_tx_int(a);
//        m_usb_tx_string("\tg\t");
//        m_usb_tx_int(g);
//        m_usb_tx_string("\tangle\t");
//        m_usb_tx_int(angle);
        m_usb_tx_string("\n");
        
        
    }
}




