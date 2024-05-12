#ifndef MAIN_H_
#define MAIN_H_


#include "tm4c123gh6pm.h"
#include "HAL/Buttons.h"
#include "HAL/MagneticSensor.h"
#include "HAL/FumeSensor.h"
#include "HAL/UltraSonic.h"
#include "MCAL/UART.h"
#include "MCAL/Systick_Functions.h"


//use tivaware for delayMs function
#include "driverlib/sysctl.h"

#define Enable_Exceptions() __asm(" CPSIE I ")
#define Disable_Exceptions() __asm(" CPSID I ")

//3amlna al flag da extern 3lshan ne3rf nest5dmo fy al ISR fy File "Buttons.c"
//w 3amalna volatile 3lshan al compiler my3mloush optimization khales wana ab2a 3ndy al full control lel flag da
extern volatile uint8 flag;

//3amlna al function extern 3lshan bnst5dmaha fy al ISR fy File "Buttons.c" w kman fy file "UltraSonic.c"
extern void delayMs(uint32 ui32Ms);



#endif /* MAIN_H_ */
