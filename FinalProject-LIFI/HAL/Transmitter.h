
#ifndef TRANSMITTER_H_
#define TRANSMITTER_H_


#define EnableTransmitter() (GPIO_PORTA_DATA_R |= (1 << 2))
#define DisableTransmitter() (GPIO_PORTA_DATA_R &= ~(1 << 2))

//PA2
void InfraTransmitter_Init(void);



#endif /* TRANSMITTER_H_ */
