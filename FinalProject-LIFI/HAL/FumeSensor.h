#ifndef FUMESENSOR_H_
#define FUMESENSOR_H_

#define FumeSensor (GPIO_PORTD_DATA_R & (1 << 3))

#include "std_types.h"

//PD3
void GasSensor_Init(void);

//howa fel 3ady be 1
//w lama bye2raa bytala3 3al pin aly mtwasaala be 0
uint8 GasSensor_GetStatus(void);

#endif /* FUMESENSOR_H_ */
