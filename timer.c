#include <msp430.h>
#include "adc.h"

extern char timeToWriteToTheScreen;
extern unsigned long timeInSeconds; // keep track of time


#pragma vector=TIMER2_A0_VECTOR
__interrupt void Timer_A2_ISR(void){
    startADCConversion();
}


void setupTimerA2(){
    TA2CTL = TASSEL_1 | MC_1 | ID_0; // ACLK, UP MODE, INPUT DIVIDE BY 2^0
}

void timerA2InterruptEnable(){
    TA2CCTL0 = CCIE;
}
void timerA2InterruptDisable(){
    TA2CCTL0 &= ~CCIE;
}
void setMaxCount(unsigned int count){
    TA2CCR0 = count;
}

void set50HzTriangleWave(){
    TA0CTL = TASSEL_1 | MC_3 | ID_0;
    TA0CCR0 = 328;
}

void set100HzSawtoothWave(){
    TA0CTL = TASSEL_1 | MC_1 | ID_0;
    TA0CCR0 = 324;
}

void set100HzSquareWave(){
    TA0CTL = TASSEL_1 | MC_1 | ID_0;
    TA0CCR0 = 164;
}
