/* Name: main.c
 * Author: <AKSHAY VARIK>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include "m_general.h"
#include <avr/interrupt.h>
#include "m_bus.h"
#include "m_imu.h"
#include "m_usb.h"

#define dt 0.001


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
volatile int ay_offset = 100;
volatile int ay_scale = 2;
volatile int ay_refined = 0;
volatile int az;
volatile int move = 0;
volatile float angle = 0;
volatile float a_home;
volatile float error;
volatile float kp = 2;
volatile float kd = 1;
volatile float ki = 1.5;
volatile float a_new=0;
volatile float sum_error=0;
volatile float error_last=0;
volatile float target_angle = 0;
volatile float gx_filtered = 0;
volatile float gx_filtered_prev = 0;
volatile float gx_filtered_latest = 0;
volatile float gx_filtered_old = 0;


int main(void)
{
m_clockdivide(3); //2MHz

//Timer
clear(TCCR1B,CS12);
set(TCCR1B,CS11);
clear(TCCR1B,CS10); //8 precale timer

set(TCCR1B,WGM13);
set(TCCR1B,WGM12);
set(TCCR1A,WGM11);
set(TCCR1A,WGM10);

set(TCCR1A,COM1B1);
clear(TCCR1A,COM1B0);

m_usb_init();

m_imu_init(0,0);

OCR1A = 250;


//read=1;

while(1){

read = m_imu_raw(data);

ax = data[0];
ay = data[1];
az = data[2];
gx = data[3];
gy = data[4];
gz = data[5];

gx_filtered=gx_filtered_prev+gx*dt;
gx_filtered_latest = 0.99*(gx_filtered_old + (gx_filtered-gx_filtered_prev));
//gx_previous = gx;
//gx = gx_previous

//ay_filtered = 0.99*ay_filtered + 0.01*ay;

//a = pow((pow(ax,2) + pow(ay,2) + pow(az,2)),0.5);
//g = pow((pow(gx,2) + pow(gy,2) + pow(gz,2)),0.5);

angle = 0.98*(angle+(gx_filtered*dt))+0.02*ay;


// ay_refined = (ay - ay_offset)*ay_scale;

error = target_angle-angle;
az = (kp * error) + (kd*(error-error_last)/dt) + (ki*sum_error*dt);
OCR1B = abs((kp * error) + (kd*(error-error_last)/dt) + (ki*sum_error*dt))*250/15000;
error_last = error;
sum_error = sum_error + error;
move = OCR1B;


if (angle>0) {
m_red(OFF);
//OCR1B = 65535 - (65535/12500)*abs(move);

clear(DDRB,1);
clear(PORTB,1);

set(DDRB,2);
set(PORTB,2);

set(DDRD,4);
set(PORTD,4);

clear(DDRD,6);
clear(PORTD,6);
} else {
m_red(ON);
//OCR1B = 65535 - (65535/12500)*abs(move);


set(DDRB,1);
set(PORTB,1);

clear(DDRB,2);
clear(PORTB,2);


set(DDRD,6);
set(PORTD,6);

clear(DDRD,4);
clear(PORTD,4);
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
m_usb_tx_string("\toffset\t");
m_usb_tx_int(gz);
//        m_usb_tx_string("\ta\t");
//        m_usb_tx_int(a);
//        m_usb_tx_string("\tg\t");
//        m_usb_tx_int(g);
m_usb_tx_string("\tmove\t");
m_usb_tx_int(move);
m_usb_tx_string("\tgx_filtered\t");
m_usb_tx_int(gx_filtered);
m_usb_tx_string("\n");


}
}