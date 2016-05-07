/*
 * Robockey.c
 *
 * Created: 11/23/2015 1:05:39 AM
 * Author : Akshay
 */ 

#include <avr/io.h>
#include "m_general.h"
#include <avr/interrupt.h>
#include "m_bus.h"
#include "m_imu.h"
#include "m_rf.h"
#include "m_usb.h"
#include "m_wii.h"

#define CHANNEL 1
#define RXADDRESS 92
#define PACKET_LENGTH 10


#define AB 13
#define AC 29
#define AD 16
#define BC 26
#define BD 23.0825
#define CD 20
#define PI 3.14159265


volatile int b = 0;
volatile int permutation[8];
volatile float angles[2];
volatile float coords_angle[3];

volatile float theta;
volatile float theta_deg;
volatile float target_angle;
volatile float diff;
volatile int at_goal;


int a = 1;
volatile int q = 0;

volatile int play_com = 0;

unsigned char buffer[PACKET_LENGTH] = {0,0,0,0,0,0,0,0,0,0};

int* permute_dists(int assign, volatile int* X_i)
{
    
    int x1 = X_i[0];
    int x2 = X_i[1];
    int x3 = X_i[2];
    int x4 = X_i[3];
    int y1 = X_i[4];
    int y2 = X_i[5];
    int y3 = X_i[6];
    int y4 = X_i[7];
    
    int pa_x[24][4] = {{x1,x2,x3,x4}, {x2,x1,x3,x4}, {x3,x2,x1,x4}, {x4,x2,x3,x1},
        {x1,x2,x4,x3}, {x2,x1,x4,x3}, {x3,x2,x4,x1}, {x4,x2,x1,x3},
        {x1,x3,x2,x4}, {x2,x3,x1,x4}, {x3,x1,x2,x4}, {x4,x3,x2,x1},
        {x1,x3,x4,x2}, {x2,x3,x4,x1}, {x3,x1,x4,x2}, {x4,x3,x1,x2},
        {x1,x4,x2,x3}, {x2,x4,x1,x3}, {x3,x4,x2,x1}, {x4,x1,x2,x3},
        {x1,x4,x3,x2}, {x2,x4,x3,x1}, {x3,x4,x1,x2}, {x4,x1,x3,x2}};
    
    int pa_y[24][4] = {{y1,y2,y3,y4}, {y2,y1,y3,y4}, {y3,y2,y1,y4}, {y4,y2,y3,y1},
        {y1,y2,y4,y3}, {y2,y1,y4,y3}, {y3,y2,y4,y1}, {y4,y2,y1,y3},
        {y1,y3,y2,y4}, {y2,y3,y1,y4}, {y3,y1,y2,y4}, {y4,y3,y2,y1},
        {y1,y3,y4,y2}, {y2,y3,y4,y1}, {y3,y1,y4,y2}, {y4,y3,y1,y2},
        {y1,y4,y2,y3}, {y2,y4,y1,y3}, {y3,y4,y2,y1}, {y4,y1,y2,y3},
        {y1,y4,y3,y2}, {y2,y4,y3,y1}, {y3,y4,y1,y2}, {y4,y1,y3,y2}};
    
    permutation[0] = pa_x[assign][0];
    permutation[1] = pa_x[assign][1];
    permutation[2] = pa_x[assign][2];
    permutation[3] = pa_x[assign][3];
    
    permutation[4] = pa_y[assign][0];
    permutation[5] = pa_y[assign][1];
    permutation[6] = pa_y[assign][2];
    permutation[7] = pa_y[assign][3];
    
    return permutation;
}


float distance(int x1, int y1, int x2, int y2)
{
    float dist;
    dist = pow((pow((x1-x2),2) + pow((y1-y2),2)),0.5);
    return dist;
}



int loss(int X_i, float star_length)
{

    float ratio;
    float min_score = 10000;
    int min_assignment;
    int* permutation;

    float AB_p;
    float AC_p;
    float AD_p;
    float BC_p;
    float BD_p;
    float CD_p;

    int i;

    ratio = star_length / AC;

    AB_p = AB * ratio;
    AC_p = AC * ratio;
    AD_p = AD * ratio;
    BC_p = BC * ratio;
    BD_p = BD * ratio;
    CD_p = CD * ratio;
    
    for (i = 0; i < 24; i++)
    {
        int x1;
        int x2;
        int x3;
        int x4;
        int y1;
        int y2;
        int y3;
        int y4;
        
        float dist_12;
        float dist_13;
        float dist_14;
        float dist_23;
        float dist_24;
        float dist_34;

        volatile float score;

        permutation = permute_dists(i, X_i);

        x1 = permutation[0];
        x2 = permutation[1];
        x3 = permutation[2];
        x4 = permutation[3];
        y1 = permutation[4];
        y2 = permutation[5];
        y3 = permutation[6];
        y4 = permutation[7];

        dist_12 = distance(x1, y1, x2, y2);
        dist_13 = distance(x1, y1, x3, y3);
        dist_14 = distance(x1, y1, x4, y4);
        dist_23 = distance(x2, y2, x3, y3);
        dist_24 = distance(x2, y2, x4, y4);
        dist_34 = distance(x3, y3, x4, y4);
        
        score = abs(dist_12 - AB_p) + abs(dist_13 - AC_p) + abs(dist_14 - AD_p) +
        abs(dist_23 - BC_p) + abs(dist_24 - BD_p) + abs(dist_34 - CD_p);

        if (score < min_score)
        {
            min_score = score;
            min_assignment = i;
        }

    }
    
    return min_assignment;

}

float adjust_angle(float angle, float vect_1, float vect_2) {
    
    float angle_adj;
    
    if (vect_1 >= 0 && vect_2 >= 0) {
        angle_adj = angle;
    } else if (vect_1 >= 0 && vect_2 < 0) {
        angle_adj = angle;
    } else if (vect_1 < 0 && vect_2 >= 0) {
        angle_adj = angle + 180;
    } else if (vect_1 < 0 && vect_2 < 0) {
        angle_adj = angle + 180;
    }
    
    return angle_adj;
    
}


float* calc_angle(float xc, float yc, float new_x3, float new_y3)
{
    int screen_mid_x = 512;
    int screen_mid_y = 384;
    float dot;
    
    float angle;
    double mid_angle;
    float x3_angle;
    
    float angle_d;
    float mid_angle_d;
    float x3_angle_d;
    
    float mid_angle_adj;
    float x3_angle_adj;
    
    float diff;

    dot = (screen_mid_x - xc)*(new_x3 - xc) + (screen_mid_y - yc)*(new_y3 - yc);
    angle = acos(dot / ((distance(screen_mid_x,screen_mid_y,xc,yc)*(distance(new_x3,new_y3,xc,yc)))));
    
    mid_angle = atan((screen_mid_y - yc) / (screen_mid_x - xc));
    x3_angle = atan((new_y3 - yc) / (new_x3 - xc));
    
    angle_d = angle * (180.0 / PI);
    mid_angle_d = mid_angle * (180.0 / PI);
    x3_angle_d = x3_angle * (180.0 / PI);
    
    mid_angle_adj = adjust_angle(mid_angle_d, screen_mid_x - xc, screen_mid_y - yc);
    x3_angle_adj = adjust_angle(x3_angle_d, new_x3 - xc, new_y3 - yc);
    
    diff = mid_angle_adj - x3_angle_adj;
    
    if (diff < 180 && diff > 0) {
        angle_d = angle_d;
    } else if (diff > -180 && diff < 0) {
        angle_d = -angle_d;
    } else if (diff > 180 && mid_angle_adj > 180) {
        angle_d = -angle_d;
    } else if (diff < -180 && x3_angle_adj > 180) {
        angle_d = angle_d;
    }
    
    angles[0] = angle_d;
    angles[1] = x3_angle_adj;
    
    return angles;
}

float* find_bot(float dist_cm, float angle) {
    
    float new_angle;
    float new_angle_R;
    float x_b;
    float y_b;
    
    if (angle < -90 && angle > -180) {
        new_angle = -(angle + 90);
        new_angle_R = (PI / 180.0) * new_angle;
        x_b = 115 - (cos(new_angle_R) * dist_cm);
        y_b = 60 + (sin(new_angle_R) * dist_cm);
    
    } else if (angle < 0 && angle > -90) {
        new_angle = -angle;
        new_angle_R = (PI / 180.0) * new_angle;
        x_b = 115 - (sin(new_angle_R) * dist_cm);
        y_b = 60 - (cos(new_angle_R) * dist_cm);
    
    } else if (angle > 0 && angle < 90) {
        new_angle = angle;
        new_angle_R = (PI / 180.0) * new_angle;
        x_b = 115 + (sin(new_angle_R) * dist_cm);
        y_b = 60 - (cos(new_angle_R) * dist_cm);
        
    } else if (angle > 90 && angle < 180) {
        new_angle = angle - 90;
        new_angle_R = (PI / 180.0) * new_angle;
        x_b = 115 + (cos(new_angle_R) * dist_cm);
        y_b = 60 + (sin(new_angle_R) * dist_cm);
    }
    
    coords_angle[0] = x_b;
    coords_angle[1] = y_b;
    coords_angle[2] = new_angle;

    return coords_angle;
}


int main(void)
{
    int assign_test;
    int* perm;
    int new_x1;
    int new_y1;
    int new_x3;
    int new_y3;
    int xc;
    int yc;
    volatile int X_i[8];
    
    float* angles_return;
    float p_dist_from_c;
    float p_dist;
    float dist_cm;
    
    float* coords_angle_return;
    int bot_angle_x3;
    int bot_angle;
    int blobs[12] = {0};
    char IR_data;
    char ir;
    
    float xb;
    float yb;
    float xg;
    float yg;
    
    int first_star_flag = 0;
    

    
    
    
    m_clockdivide(0); //2MHz
    
    m_bus_init();
    m_usb_init();

    
    m_rf_open(CHANNEL,RXADDRESS,PACKET_LENGTH);
//    set(EIMSK,INT2);
    
    
//    //Timer
//    clear(TCCR1B,CS12);
//    set(TCCR1B,CS11);
//    clear(TCCR1B,CS10); //8 precale timer
//    
//    set(TCCR1B,WGM13);
//    set(TCCR1B,WGM12);
//    set(TCCR1A,WGM11);
//    set(TCCR1A,WGM10);
//    
//    set(TCCR1A,COM1B1);
//    clear(TCCR1A,COM1B0);
//    
//    OCR1A = 250;
    
    
//    clear(TCCR1B,CS12);
//    clear(TCCR1B,CS11);
//    set(TCCR1B,CS10);
//    
//    set(TCCR1B,WGM13);
//    set(TCCR1B,WGM12);
//    set(TCCR1A,WGM11);
//    set(TCCR1A,WGM10);
//    
//    set(TCCR1A,COM1B1);
//    clear(TCCR1A,COM1B0);
    
    clear(TCCR3B,CS32);
    clear(TCCR3B,CS31);
    set(TCCR3B,CS30);
    
    set(TCCR3B,WGM33);
    set(TCCR3B,WGM32);
    set(TCCR3A,WGM31);
    clear(TCCR3A,WGM30);
    
    set(TCCR3A,COM3A1);
    clear(TCCR3A,COM3A0);
    
    clear(DDRC,6);

    ir = m_wii_open();
    
    sei();
    
    
    
    while(1)
    {
        
//        if (play_com == 0) {
//            continue;
////            m_red(TOGGLE);
////            m_wait(100);
//        }
        
        IR_data = m_wii_read(blobs);
        
        X_i[0] = blobs[0];
        X_i[1] = blobs[3];
        X_i[2] = blobs[6];
        X_i[3] = blobs[9];
        X_i[4] = blobs[1];
        X_i[5] = blobs[4];
        X_i[6] = blobs[7];
        X_i[7] = blobs[10];
        
        m_usb_tx_int(blobs[0]);
        m_usb_tx_string(" x \n");
        m_usb_tx_int(blobs[1]);
        m_usb_tx_string(" y \n");
        
        
        
        if (!((X_i[4] == 1023) || (X_i[5] == 1023) || (X_i[6] == 1023) || (X_i[7] == 1023)))
        {
            
            assign_test = loss(X_i, 100.0);
            perm = permute_dists(assign_test, X_i);
            
            new_x1 = perm[0];
            new_x3 = perm[2];
            new_y1 = perm[4];
            new_y3 = perm[6];
            
            xc = (new_x1+new_x3)/2;
            yc = (new_y1+new_y3)/2;
            
            angles_return = calc_angle(xc, yc, new_x3, new_y3);
            
            p_dist_from_c = distance(512, 384, xc, yc);
            p_dist = distance(new_x1, new_y1, new_x3, new_y3);
            dist_cm = p_dist_from_c * (29 / p_dist);
            
            coords_angle_return = find_bot(dist_cm, angles_return[0]);
      
            bot_angle_x3 = ((int)angles[1] + 90) % 360;
            bot_angle = (360-((180+bot_angle_x3)%360));
            
            xb = coords_angle_return[0];
            yb = coords_angle_return[1];
            
            if ((xb > 115) && (first_star_flag == 0)) {
                first_star_flag = 1;
            } else if ((xb <= 115) && (first_star_flag == 0)){
                first_star_flag = 2;
            }
            
            m_usb_tx_int(xb);
            m_usb_tx_string(" xb \n");
            m_usb_tx_int(yb);
            m_usb_tx_string(" yb \n");
            
        } else {
            continue;
        }
        
        
        if (first_star_flag == 1) {
            xg = 0;
            yg = 60;
        } else if (first_star_flag == 2){
            xg = 230;
            yg = 60;
        }
        
        
        
        if (xb >= xg) {

            if (yb > 60) {
                theta = atan((yb - yg)/(xb - xg));
                theta_deg = (180 / PI) * theta;

                target_angle = 90 - theta_deg;
            } else {
                theta = atan((yg - yb)/(xb - xg));
                theta_deg = (180 / PI) * theta;

                target_angle = 90 + theta_deg;
            }
        } else {
            
            if (yb > 60) {
                theta = atan((yb - yg)/(xg - xb));
                theta_deg = (180 / PI) * theta;
                
                target_angle = 270 + theta_deg;
            } else {
                theta = atan((yg - yb)/(xg - xb));
                theta_deg = (180 / PI) * theta;
                
                target_angle = 270 - theta_deg;
            }
        }
        
        bot_angle = 360 - bot_angle;
        
        // motor drivers
        
        diff = bot_angle - target_angle;
        if (diff < 180 && diff >= 0) {
            diff = diff;
        } else if (diff < -180) {
            diff = (360 + diff);
        } else if (diff < 0 && diff >= -180) {
            diff = diff;
        }

//        diff = 20;
        
        m_usb_tx_int(xg);
        m_usb_tx_string(" xg \n");
        m_usb_tx_int(yg);
        m_usb_tx_string(" yg \n");

        
        m_usb_tx_int(theta_deg);
        m_usb_tx_string(" theta_deg \n");

        
        m_usb_tx_int(target_angle);
        m_usb_tx_string(" target angle \n");
        
        m_usb_tx_int(bot_angle);
        m_usb_tx_string(" bot_angle \n");

        
        m_usb_tx_int(diff);
        m_usb_tx_string(" diff \n");

        
        if (first_star_flag) {
            at_goal = (xb < 20) && (yb > 40 && yb < 80);
        } else if (first_star_flag == 2) {
            at_goal = (xb > 210) && (yb > 40 && yb < 80);
        }

        set(DDRC,6);
        ICR3 = 10000;

        
        if (diff >= 10 && !at_goal) {
            m_red(ON);
            m_green(OFF);
            
            OCR3A = 3500;
            
            set(DDRB,2);
            set(PORTB,2);
    
            clear(DDRC,7);
            clear(PORTC,7);
            
        } else if (diff <= -10 && !at_goal) {
            m_red(ON);
            m_green(OFF);
            
            OCR3A = 3500;
            
            set(DDRB,2);
            set(PORTB,2);
            
            set(DDRC,7);
            set(PORTC,7);
            

            
        } else if ((diff < 10 || diff > -10) && !at_goal) {
           
            OCR3A = 3500;
            
            m_green(ON);
             m_red(OFF);
            
            clear(DDRB,2);
            clear(PORTB,2);
            
            clear(DDRC,7);
            clear(PORTC,7);
            
        } else {
            m_red(ON);
            m_green(ON);
            
            OCR3A = 0;
            
            m_green(OFF);
            
            clear(DDRC,6);
            
            clear(DDRB,2);
            clear(PORTB,2);
            
            clear(DDRC,7);
            clear(PORTC,7);
            
        }
    
    }
}



ISR(INT2_vect)
{
    (m_rf_read(buffer,PACKET_LENGTH));
    
    
        if (buffer[0] == 0xA1)
        {
            play_com = 1;
//             m_green(TOGGLE);
        
        }
    
}
















