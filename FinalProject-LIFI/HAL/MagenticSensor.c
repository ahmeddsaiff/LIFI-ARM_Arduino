#include "tm4c123gh6pm.h"
#include "HAL/MagneticSensor.h"

//PD2
void MagneticSensor_Init(void)
{
    GPIO_PORTD_LOCK_R = 0x4C4F434B; /* Unlock the GPIO_PORTD_CR_R */
    GPIO_PORTD_CR_R |= (1 << 2); /* Enable changes on PD2 */
    GPIO_PORTD_AMSEL_R &= ~(1 << 2); /* Disable Analog on PD2 */
    GPIO_PORTD_PCTL_R &= 0xFFFFF0FF; /* Clear PMCx bits for PD2 to use it as GPIO pin */
    GPIO_PORTD_DIR_R &= ~(1 << 2); /* Configure PD2 as input pin */
    GPIO_PORTD_AFSEL_R &= ~(1 << 2); /* Disable alternative function on PD2 */
    GPIO_PORTD_DEN_R |= (1 << 2); /* Enable Digital I/O on PD2 */
}

//Get status of Magnetic switch
//lw la2at haytl3 1
uint8 Magnetic_GetStatus(void){
    if(MagneticSwitch)
        return 1;
    else
        return 0;
}

