#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "pic18f4321_Config.h"
#include "LCD.h" //Was initially used to test sensors and motors
#define _XTAL_FREQ 4000000

#define TRIGGER_FRONTA PORTBbits.RB0 	
#define ECHO_FRONTA PORTBbits.RB1 	
#define TRIGGER_FRONTB PORTAbits.RA0 	
#define ECHO_FRONTB PORTAbits.RA1 	
#define TRIGGER_RIGHT PORTBbits.RB2
#define ECHO_RIGHT PORTBbits.RB3
#define TRIGGER_LEFT PORTBbits.RB4
#define ECHO_LEFT PORTBbits.RB5

#define MOTORLA PORTCbits.RC0   
#define MOTORLB PORTCbits.RC1   
//RC2 is PWM Pin
#define MOTORRA PORTCbits.RC3
#define MOTORRB PORTCbits.RC4


void FORWARD();
void STILL();
void RIGHT();
void LEFT();

void FORWARD()
{
  MOTORLA = 0;
  MOTORLB = 1;  
  MOTORRA = 0;
  MOTORRB = 1;
}

void STILL()
{
  MOTORLA = 0;
  MOTORLB = 0;  
  MOTORRA = 0;
  MOTORRB = 0;
}


void RIGHT()
{
  MOTORLA = 0;
  MOTORLB = 1;  
  MOTORRA = 1;
  MOTORRB = 0;
}

void LEFT()
{
  MOTORLA = 1;
  MOTORLB = 0;  
  MOTORRA = 0;
  MOTORRB = 1; 
}


void main(void) {
OSCCON = 0x60; 
    unsigned int distanceA,distanceB,distance2,distance3;
    unsigned int timeA,timeB,time2,time3;
    ADCON1 = 0x0F; 
    T1CON =0x31; //Prescalar 1:8
    TRISC = 0; 
    TRISD = 0; 
    TRISBbits.TRISB0 = 0;    
    TRISBbits.TRISB1 = 1;  
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 1;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 1;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 1;
    TMR1 = 0;
   
       //PWM 
   T2CON = 0x07; 
   PR2 = 0xFF; 
   CCP1CON = 0x0C; 
// Adjust duty cycle (example for 40% duty cycle for forward, 100% for turning)
   CCPR1L = 0x7F; 
 
    
   
    while(1){   
  //Sensor FRONTA
    PIR1bits.TMR1IF = 0; 
    TMR1 = 0x00;  
    distanceA = 0x00;   

    TRIGGER_FRONTA = 0; 
    __delay_ms(10);
    TRIGGER_FRONTA = 1; 
    __delay_us(10);
     TRIGGER_FRONTA = 0; 
    while(ECHO_FRONTA == 0);  
    T1CONbits.TMR1ON = 1; 
    while(ECHO_FRONTA == 1); 
    T1CONbits.TMR1ON = 0;  
    timeA = TMR1; 
    distanceA = (timeA*8*0.0343)/2; 
    if(distanceA > 75)
        distanceA = 75;
    //End of SensorFRONTA
    
      __delay_ms(30);
      
      
      //Sensor FRONTB
    PIR1bits.TMR1IF = 0;
    TMR1 = 0x00;  
    distanceB = 0x00;  

    TRIGGER_FRONTB = 0;  
    __delay_ms(10);
    TRIGGER_FRONTB = 1; 
    __delay_us(10);
     TRIGGER_FRONTB = 0; 
    while(ECHO_FRONTB == 0);  
    T1CONbits.TMR1ON = 1; 
    while(ECHO_FRONTB == 1); 
    T1CONbits.TMR1ON = 0;  
    timeB = TMR1; 
    distanceB = (timeB*8*0.0343)/2;  
    if(distanceB > 150)
        distanceB = 150;
    //End of SensorFRONTB
    
      __delay_ms(30);
      
     //Sensor_Right
    PIR1bits.TMR1IF = 0; 
    TMR1 = 0x00;  
    distance2 = 0x00;   

    TRIGGER_RIGHT = 0; 
    __delay_ms(10);
    TRIGGER_RIGHT = 1; 
    __delay_us(10);
     TRIGGER_RIGHT = 0; 
    while(ECHO_RIGHT == 0);   
    T1CONbits.TMR1ON = 1;  
    while(ECHO_RIGHT == 1); 
    T1CONbits.TMR1ON = 0;   
    time2 = TMR1; 
    distance2 = (time2*8*0.0343)/2;
    if(distance2 >150)
        distance2=150;
        //End of Sensor_Right
    
    __delay_ms(30);
    
    //Sensor Left
     PIR1bits.TMR1IF = 0; //this tells us that the register did not overflow.
    TMR1 = 0x00;  
    distance3 = 0x00;   

    TRIGGER_LEFT = 0; 
    __delay_ms(10);
    TRIGGER_LEFT = 1; 
    __delay_us(10);
     TRIGGER_LEFT = 0; 
    while(ECHO_LEFT == 0);   
    T1CONbits.TMR1ON = 1;  
    while(ECHO_LEFT == 1); 
    T1CONbits.TMR1ON = 0;   
    time3 = TMR1; 
    distance3 = (time3*8*0.0343)/2;
    if(distance3 >50)
        distance3=50;
    //End of Sensor Left
    
    __delay_ms(30);
    
       CCPR1L = 0x7F; 
       
      if(distanceA > 35 || distanceB > 35)
     {
       
          FORWARD();
     }
     
      else if(distanceA < 35 || distanceB < 35)
     {
        CCPR1L = 0xFF;
         if(distance2 < distance3)
         {
           LEFT();  
         }
        
         else if(distance2 > distance3)
         {
             RIGHT();
         }
        
     }
   
    
     
         
    } 
   
}
   
 

