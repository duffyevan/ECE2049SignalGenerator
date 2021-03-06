#include <msp430.h>
#include "adc.h"
#include "peripherals.h"

extern unsigned int potValue;
extern unsigned char buttonSetting;
unsigned int adcmem1 = 0;

#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void){
        potValue = ADC12MEM0;
        adcmem1 = ADC12MEM1;
}



void setupADC(){
    REFCTL0 &= ~REFMSTR; // get ready to use internal reference
	ADC12CTL0 = ADC12MSC | ADC12SHT0_9 | ADC12REFON | ADC12ON; // set for multi sample conversion. use middle of the road sample time. Use internal reference (default 1.5v). Turn the ADC on
	ADC12CTL1 = ADC12SHP | ADC12CONSEQ_1 | ADC12CSTARTADD_0; // pulsemode. sequence of conversions. start address is 0.

    P6SEL |= BIT0 | BIT1; // enable A0 instead of P6.0

    ADC12MCTL0 = ADC12SREF_0 | ADC12INCH_0; // Sets the ref to internal reference. Selects temperature diode for channel.
    ADC12MCTL1 = ADC12INCH_1 | ADC12EOS; // set the input channel to the pot pin (A0) and set it as the end of the sequence
    enableConversionInterrupt();
}

void waitForADC(){
    while (ADC12CTL1 & ADC12BUSY);
}

void startADCConversion(){
    enableConversionInterrupt();
    ADC12CTL0 |= ADC12SC | ADC12ENC; // start conversion and enable conversion
}

int getMeasurement(){
    return ADC12MEM0 & 0x0FFF; // get the temperature measurement from the ADC
}

void enableConversionInterrupt(){
    ADC12IE |= 0x0002; // Set it so it will fire when its done converting MEM0
}

