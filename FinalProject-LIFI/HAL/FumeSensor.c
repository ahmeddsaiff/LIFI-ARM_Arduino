#include "tm4c123gh6pm.h"
#include "HAL/FumeSensor.h"

//PD3
void GasSensor_Init(void)
{
    GPIO_PORTD_LOCK_R = 0x4C4F434B; /* Unlock the GPIO_PORTD_CR_R */
    GPIO_PORTD_CR_R |= (1 << 3) | (1 << 2); /* Enable changes on PD3 */
    GPIO_PORTD_AMSEL_R &= ~(1 << 3) & ~(1 << 2); /* Disable Analog on PD3 */
    GPIO_PORTD_PCTL_R &= 0xFFFF00FF; /* Clear PMCx bits for PD3 to use it as GPIO pin */
    GPIO_PORTD_DIR_R &= ~(1 << 3) & ~(1 << 2); /* Configure PD3 as input pin */
    GPIO_PORTD_AFSEL_R &= ~(1 << 3) & ~(1 << 2); /* Disable alternative function on PD3 */
    GPIO_PORTD_DEN_R |= (1 << 3) | (1 << 2); /* Enable Digital I/O on PD3 */
}

//howa fel 3ady be 1
//w lama bye2raa bytala3 3al pin aly mtwasaala be 0
uint8 GasSensor_GetStatus(void){
    if(!(FumeSensor))
        return 1;
    else
        return 0;
}


