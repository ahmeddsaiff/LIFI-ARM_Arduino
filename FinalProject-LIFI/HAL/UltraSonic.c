#include "HAL/UltraSonic.h"
#include "tm4c123gh6pm.h"

/* global variables to store and display distance in cm */
uint32 g_distance; /* stores measured distance value */
char mesg[20]; /* string format of distance value */

void delayMs(uint32 ui32Ms);


/* This function captures consecutive rising and falling edges of a periodic signal */
/* from Timer Block 0 Timer A and returns the time difference (the period of the signal). */
uint32 Measure_distance(void)
{
    int lastEdge, thisEdge;

    /* Given 10us trigger pulse */
    GPIO_PORTA_DATA_R &= ~(1 << 4); /* make trigger  pin high */
    delayMs(10); /*10 seconds delay */
    GPIO_PORTA_DATA_R |= (1 << 4); /* make trigger  pin high */
    delayMs(10); /*10 seconds delay */
    GPIO_PORTA_DATA_R &= ~(1 << 4); /* make trigger  pin low */

    while (1)
    {
        TIMER0_ICR_R = 4; /* clear timer0A capture flag */
        while ((TIMER0_RIS_R & 4) == 0); /* wait till captured */
        if (GPIO_PORTB_DATA_R & (1 << 6)) /*check if rising edge occurs */
        {
            lastEdge = TIMER0_TAR_R; /* save the timestamp */
            /* detect falling edge */
            TIMER0_ICR_R = 4; /* clear timer0A capture flag */
            while ((TIMER0_RIS_R & 4) == 0); /* wait till captured */
            thisEdge = TIMER0_TAR_R; /* save the timestamp */
            g_distance = ((thisEdge - lastEdge) * 10625) / 10000000; /* convert pulse duration into distance */
            return g_distance; /* return the time difference */
        }
    }
}

void ultraSonic_Setup_pins(void){
    SYSCTL_RCGCGPIO_R |= 0x03; /* enable clock to PORTB & PORTA*/

    /* PB6 as digital input for ECHO pin */
    GPIO_PORTB_AMSEL_R |= 0x40; /* Enable Analog on PB6*/
//    GPIO_PORTB_PCTL_R &= ~0x0F000000;
    GPIO_PORTB_PCTL_R |= 0x07000000; /* configure PB6 for T0CCP0 */
    GPIO_PORTB_DIR_R &= ~0x40; /* make PB6 an input pin */
    GPIO_PORTB_AFSEL_R |= 0x40; /* use PB6 alternate function */
    GPIO_PORTB_DEN_R |= 0x40; /* make PB6 as input pin */


    /* PA4 as a digital output signal to provide trigger signal */
    GPIO_PORTA_AMSEL_R &= 0xEF; /* Disable Analog on PA4*/
    GPIO_PORTA_PCTL_R &= 0xFFF0FFFF; /* Clear PMCx bits for PA4 to use it as GPIO pin */
    GPIO_PORTA_DIR_R |= (1 << 4); /* set PA4 as a digial output pin */
    GPIO_PORTA_AFSEL_R &= 0xEF; /* Disable alternative function on PA2 */
    GPIO_PORTA_DEN_R |= (1 << 4); /* make PA4 as digital pin */
    GPIO_PORTA_DATA_R &= ~(1<<4); /* Clear bit 4 in Data Register to make trigger pin low*/

}

/* Timer0A initialization function */
/* Initialize Timer0A in input-edge time mode with up-count mode */
void UltraSonic_init(void)
{
    SYSCTL_RCGCTIMER_R |= 0x01; /* enable clock to Timer Block 0 */

    ultraSonic_Setup_pins();

    TIMER0_CTL_R &= ~0x01; /* disable timer0A during setup */
    TIMER0_CFG_R = 4; /* 16-bit timer mode */
    TIMER0_TAMR_R = 0x17; /* up-count, edge-time, capture mode */
    TIMER0_CTL_R |= 0x0C; /* capture the rising edge */
    TIMER0_CTL_R |= (1 << 0); /* enable timer0A */
}


