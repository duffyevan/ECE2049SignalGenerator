#include <msp430.h>

/*
 * DAC pin assignment is as follows
 * LDAC         P3.7
 * CS           P8.2
 * MOSI/SDI     P3.0
 * SCLK         P3.2
 */

#define DAC_PORT_LDAC_SEL   P3SEL
#define DAC_PORT_LDAC_DIR   P3DIR
#define DAC_PORT_LDAC_OUT   P3OUT
#define DAC_PORT_CS_SEL     P8OUT
#define DAC_PORT_CS_DIR     P8DIR
#define DAC_PORT_CS_OUT     P8OUT

#define DAC_PIN_CS      BIT2
#define DAC_PIN_LDAC        BIT7

#define DAC_SPI_REG_IFG UCB0IFG
#define DAC_SPI_REG_TXBUF   UCB0TXBUF
// â€¦ etc ...



/** **********************************************
 * Initialize the DAC and its associated SPI bus,
 * using parameters defined in peripherals.h
 ************************************************/
void DACInit(void){
    // Configure LDAC and CS for digital IO outputs
    DAC_PORT_LDAC_SEL &= ~DAC_PIN_LDAC;
    DAC_PORT_LDAC_DIR |=  DAC_PIN_LDAC;
    DAC_PORT_LDAC_OUT |= DAC_PIN_LDAC; // Deassert LDAC

    DAC_PORT_CS_SEL   &= ~DAC_PIN_CS;
    DAC_PORT_CS_DIR   |=  DAC_PIN_CS;
    DAC_PORT_CS_OUT   |=  DAC_PIN_CS;  // Deassert CS

    P3SEL |= 0x05;
}



void DACSetValue(unsigned int dac_code){
    // Start the SPI transmission by asserting CS (active low)
    // This assumes DACInit() already called
    DAC_PORT_CS_OUT &= ~DAC_PIN_CS;

    // Write in DAC configuration bits. From DAC data sheet
    // 3h=0011 to highest nibble.
    // 0=DACA, 0=buffered, 1=Gain=1, 1=Out Enbl
    dac_code |= 0x3000;     // Add control bits to DAC word

    unsigned char lo_byte = (unsigned char)(dac_code & 0x00FF);
    unsigned char hi_byte = (unsigned char)((dac_code & 0xFF00) >> 8);

    // First, send the high byte
    DAC_SPI_REG_TXBUF = hi_byte;

    // Wait for the SPI peripheral to finish transmitting
    while(!(DAC_SPI_REG_IFG & UCTXIFG)) {
        _no_operation();
    }

    // Then send the low byte
    DAC_SPI_REG_TXBUF = lo_byte;

    // Wait for the SPI peripheral to finish transmitting
    while(!(DAC_SPI_REG_IFG & UCTXIFG)) {
        _no_operation();
    }

    // We are done transmitting, so de-assert CS (set = 1)
    DAC_PORT_CS_OUT |=  DAC_PIN_CS;

    // This DAC is designed such that the code we send does not
    // take effect on the output until we toggle the LDAC pin.
    // This is because the DAC has multiple outputs. This design
    // enables a user to send voltage codes to each output and
    // have them all take effect at the same time.
    DAC_PORT_LDAC_OUT &= ~DAC_PIN_LDAC;  // Assert LDAC
     __delay_cycles(10);                 // small delay
    DAC_PORT_LDAC_OUT |=  DAC_PIN_LDAC;  // De-assert LDAC
}
