#include "tm4c123gh6pm.h"
#include "Systick_Functions.h"

/* Enable the SystTick Timer to run using the System Clock with Frequency 16Mhz and Count half second */
void SysTick_Init(void)
{
    SystickTickDisable();              /* Disable the SysTick Timer by Clear the ENABLE Bit */
    SystickPeriodSet(500);       /* Set the Reload value with 15999999 to count one Second */
    NVIC_ST_CURRENT_R = 0;              /* Clear the Current Rister value */
    SystickTickEnable();
}

void SystickTickDisable(void){
    NVIC_ST_CTRL_R    = 0;
}

void SystickTickEnable(void){
    /* Configure the SysTick Control Rister
     * Enable the SysTick Timer (ENABLE = 1)
     * Enable SysTick Interrupt (INTEN = 1)
     * Choose the clock source to be System Clock (CLK_SRC = 1) */
    NVIC_ST_CTRL_R   |= 0x07;
//    NVIC_SYSTEM_PRI3_R |= (0x4 << 29);   //set priority
}

uint32 SystickPeriodGet(void){
    return (NVIC_ST_RELOAD_R + 1) / 16000;
}

void SystickPeriodSet(uint32 Period){
    NVIC_ST_RELOAD_R = Period * 16000 - 1;
}

uint32 SystickValueGet(void){
    return (NVIC_ST_CURRENT_R);
}
//boolean is an -> typedef uint8//
boolean Systick_Is_TimeOut(void){
    return (NVIC_ST_CTRL_R & 0x10000) == 0x10000;
}




