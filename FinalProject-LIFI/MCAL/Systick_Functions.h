#ifndef SYSTICK_FUNCTIONS_H_
#define SYSTICK_FUNCTIONS_H_

#include "std_types.h"

void SysTick_Init(void);
void SystickTickDisable(void);
void SystickTickEnable(void);
uint32 SystickPeriodGet(void);
void SystickPeriodSet(uint32 Period);
uint32 SystickValueGet(void);
boolean Systick_Is_TimeOut(void);



#endif /* SYSTICK_FUNCTIONS_H_ */
