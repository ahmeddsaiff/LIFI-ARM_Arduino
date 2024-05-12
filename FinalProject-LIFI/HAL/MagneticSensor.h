#include "std_types.h"

#ifndef MAGNETICSENSOR_H_
#define MAGNETICSENSOR_H_

#define MagneticSwitch (GPIO_PORTD_DATA_R & (1 << 2))

//PD2
void MagneticSensor_Init(void);

//Get status of Magnetic switch
//lw la2at haytl3 1
uint8 Magnetic_GetStatus(void);


#endif /* MAGNETICSENSOR_H_ */
