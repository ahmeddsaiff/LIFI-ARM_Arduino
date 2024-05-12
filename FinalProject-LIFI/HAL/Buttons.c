#include "tm4c123gh6pm.h"
#include "HAL/Buttons.h"
#include "MCAL/UART.h"


uint8 flag;
void delayMs(uint32 ui32Ms);


void ISR_PORTF(void)
{
    //checking if the flag of the desired pin is raised "0" or not
    //if the stop button is pressed ->
    //                                 enable the trnasmitter for 2.5 secs to transmit data
    //                                   then disable the transmitter
    //
    if ((GPIO_PORTF_ICR_R & 0x01) == 0)
    {
        EnableTransmitter();
        delayMs(2500);
        DisableTransmitter();

        //al system fel awl closed w da hanefdal fee lehad ma nedous start
        UART5_SendString("Closed   zzzzz  ");

        //make the flag = 0 :
        //                   3lshan nekhrog mn al loop aly fel main,
        //                   w nestana lama al start button yedas tany 3lshan al system yesht8l
        flag = 0;
        //put in the flag Rister "1" to make it equal 0
        GPIO_PORTF_ICR_R |= (1 << 0);
    }
}

//PF0 -> in tiva
void StopButton_Init(void)
{
//    //enable the priority for switch
//    NVIC_PRI7_R &= ~(0x7 << 21);
//    //give the switch it's priority = 2 "msln"
//    NVIC_PRI7_R |= (0x3 << 21);

    //Enable pin 0 in portf --> rakamaha 30 mn al datasheet
    NVIC_EN0_R |= (1 << 30);

    //clear GPIOIS to detect edge
    GPIO_PORTF_IS_R &= ~(1 << 0);

    //clear GPIOIBE to make it only interrupt with the falling edge
    GPIO_PORTF_IBE_R &= ~(1 << 0);

    //clear GPIOIEV to detect falling edge
    GPIO_PORTF_IEV_R &= ~(1 << 0);

    GPIO_PORTF_IM_R |= (1 << 0);

    GPIO_PORTF_RIS_R |= (1 << 0);

    GPIO_PORTF_LOCK_R = 0x4C4F434B; /* Unlock the GPIO_PORTF_CR_R */
    GPIO_PORTF_CR_R |= (1 << 0); /* Enable changes on PF0 */
    GPIO_PORTF_AMSEL_R &= ~(1 << 4); /* Disable Analog on PF0 */
    GPIO_PORTF_PCTL_R &= 0xFFFFFFF0; /* Clear PMCx bits for PF0 to use it as GPIO pin */
    GPIO_PORTF_DIR_R &= ~(1 << 0); /* Configure PF0 as input pin */
    GPIO_PORTF_AFSEL_R &= ~(1 << 0); /* Disable alternative function on PF0 */
    GPIO_PORTF_PUR_R |= (1 << 0); /* Enable pull-up on PF0 */
    GPIO_PORTF_DEN_R |= (1 << 0); /* Enable Digital I/O on PF0 */
}

/*Enable PB1 */
void StartButton_Init(void)
{
    GPIO_PORTB_LOCK_R = 0x4C4F434B; /* Unlock the GPIO_PORTB_CR_R */
    GPIO_PORTB_CR_R |= (1 << 1); /* Enable changes on PB1 */
    GPIO_PORTB_AMSEL_R &= ~(1 << 1); /* Disable Analog on PB1 */
    GPIO_PORTB_PCTL_R &= 0xFFFFFF0F; /* Clear PMCx bits for PB1 to use it as GPIO pin */
    GPIO_PORTB_DIR_R &= ~(1 << 1); /* Configure PB1 as input pin */
    GPIO_PORTB_AFSEL_R &= ~(1 << 1); /* Disable alternative function on PB1 */
    GPIO_PORTB_DEN_R |= (1 << 1); /* Enable Digital I/O on PB1 */
}


//Return the status for the buttons (1 aw 0)
uint8 Get_StartButton_Status(void){
    if(startButton)
        return 1;
    else
        return 0;
}
