/************** ECE2049 DEMO CODE ******************/
/**************  20 August 2016   ******************/
/***************************************************/

#include <msp430.h>

/* Peripherals.c and .h are where the functions that implement
 * the LEDs and keypad, etc are. It is often useful to organize
 * your code by putting like functions together in files.
 * You include the header associated with that file(s)
 * into the main file of your project. */
#include "peripherals.h"
#include "adc.h"
#include "dac.h"
#include "timer.h"
#include "buttons.h"
// Function Prototypes
void swDelay(char numLoops);

// Declare globals here
unsigned int potValue = 0;
unsigned char buttonSetting = 1;

// Main
void main(void)

{

    WDTCTL = WDTPW | WDTHOLD;       // Stop watchdog timer
    _BIS_SR(GIE); // global interrupt enable
    configDisplay(); // init the graphics lib
    Graphics_drawStringCentered(&g_sContext, "Welcome", AUTO_STRING_LENGTH, 40,15, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, "S1 = SQUARE", AUTO_STRING_LENGTH,40, 25, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, "S2 = SAW", AUTO_STRING_LENGTH,40, 35, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, "S3 = TRIANGLE", AUTO_STRING_LENGTH,40, 45, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, "S4 = DC (Var)", AUTO_STRING_LENGTH,40, 55, OPAQUE_TEXT);

    Graphics_flushBuffer(&g_sContext);
    DACInit();
    configureButtons();
    setupADC();
    setupTimerA2();
    setMaxCount(654);
    timerA2InterruptEnable();
    while (1)
    {
        switch (buttonSetting)
        {
        case 1:
            DACSetValue(potValue);
            break;
        case 4:
        case 2:
            DACSetValue(TA0R * 12); //(12 * potValue / 4096)
            break;
        case 8:
            DACSetValue((TA0R>164)*4095);
        }
        int temp;
        if (temp = getButtons())
        {
            buttonSetting = temp;
            switch (buttonSetting)
            {
            case 1: // DC
                break;
            case 2: // triangle wave
                set50HzTriangleWave();
                break;

            case 4: // sawtooth
                set100HzSawtoothWave();
                break;
            case 8:
                set100HzSawtoothWave();
                break;
            }

        }
    }

}

void swDelay(char numLoops)
{
    // This function is a software delay. It performs
    // useless loops to waste a bit of time
    //
    // Input: numLoops = number of delay loops to execute
    // Output: none
    //
    // smj, ECE2049, 25 Aug 2013

    volatile unsigned int i, j;	// volatile to prevent optimization
                                // by compiler

    for (j = 0; j < numLoops; j++)
    {
        i = 50000;					// SW Delay
        while (i > 0)				// could also have used while (i)
            i--;
    }
}
