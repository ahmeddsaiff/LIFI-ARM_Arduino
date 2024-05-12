#ifndef BUTTONS_H_
#define BUTTONS_H_

#define startButton (GPIO_PORTB_DATA_R & (1 << 1))

#include "HAL/Transmitter.h"
#include "std_types.h"


//PF0 -> in tiva
void StopButton_Init(void);

/*PB1 */
void StartButton_Init(void);

//Return the status for the Start button (1 aw 0)
uint8 Get_StartButton_Status(void);

#endif /* BUTTONS_H_ */
