#include "tm4c123gh6pm.h"
#include "HAL/Transmitter.h"

//PA2
void InfraTransmitter_Init(void)
{
    GPIO_PORTA_AMSEL_R &= 0xFB; /* Disable Analog on PA2*/
    GPIO_PORTA_PCTL_R &= 0xFFFFF0FF; /* Clear PMCx bits for PA2 to use it as GPIO pin */
    GPIO_PORTA_DIR_R |= 0x04; /* Configure PA2 as output pin */
    GPIO_PORTA_AFSEL_R &= 0xFB; /* Disable alternative function on PA2 */
    GPIO_PORTA_DEN_R |= 0x04; /* Enable Digital I/O on PA2 */
    GPIO_PORTA_DATA_R &= 0xFB; /* Clear bit 2 in Data Risters to turn off the led */
}
