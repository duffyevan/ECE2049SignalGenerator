#include <msp430.h>


void configureButtons(){
    // Seleciton (DI/O)
    P7SEL &= ~0x11; // ~0001 0001
    P3SEL &= ~0x40; // ~0100 0000
    P2SEL &= ~0x04; // ~0000 0100

    // Direction (INPUT)
    P7DIR &= ~0x11; // ~0001 0001
    P3DIR &= ~0x40; // ~0100 0000
    P2DIR &= ~0x04; // ~0000 0100

    // Resistor Enable (ENABLED)
    P7REN |= 0x11; // 0001 0001
    P3REN |= 0x40; // 0100 0000
    P2REN |= 0x04; // 0000 0100

    // Resistor Direction (PULL UP)
    P7OUT |= 0x11; // 0001 0001
    P3OUT |= 0x40; // 0100 0000
    P2OUT |= 0x04; // 0000 0100
}

char getButtons(){
    return ~((P7IN&1)<<3 | ((P3IN&0x40)>>4) | (P2IN&0x04)>>1 | ((P7IN&0x10)>>4))&0x0F;
    //      P7.0 In     P3.6 in as bit 1
    //                      P2.2 in as bit 2
    //                                  P7.4 in as bit 3
}
