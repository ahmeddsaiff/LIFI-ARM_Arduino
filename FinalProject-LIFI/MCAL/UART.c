#include "UART.h"
#include "tm4c123gh6pm.h"

static void GPIO_SetupUART5Pins(void)
{
    SYSCTL_RCGCGPIO_R |= 0x10; /* Enable clock for GPIO PORTC */
    while (!(SYSCTL_RCGCGPIO_R & 0x10)); /* Wait until GPIO PORTC clock is activated and it is ready for access*/

    GPIO_PORTE_AMSEL_R &= 0xCF; /* Disable Analog on PC4 & PC5 */
//    GPIO_PORTE_DIR_R &= 0xEF; /* Configure PC4 as input pin */
//    GPIO_PORTE_DIR_R |= 0x20; /* Configure PC5 as output pin */
    GPIO_PORTE_AFSEL_R |= 0x30; /* Enable alternative function on PC4 & PC5 */
    /* Set PMCx bits for PC4 & PC5 with value 1 to use PC4 as UART0 RX pin and PC5 as UART0 Tx pin */
    GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R & 0xFF00FFFF) | 0x00110000;
    GPIO_PORTE_DEN_R |= 0x30; /* Enable Digital I/O on PC4 & PC5 */
}

void UART5_Init(void)
{
    /* Setup UART5 pins:
     * PC4 --> U0RX & PC5 --> U0TX */
    GPIO_SetupUART5Pins();

    SYSCTL_RCGCUART_R |= 0x20; /* Enable clock for UART5 */
    while (!(SYSCTL_RCGCUART_R & 0x20)); /* Wait until UART5 clock is activated and it is ready for access*/

    /* Disable UART5 at the beginning ;
     * by clearing UARTEN bit */
    UART5_CTL_R = 0;

    //bit(0:3) only
    //rest bits are reserved bits
    UART5_CC_R = 0; /* Use System Clock*/

    /* To Configure UART5 with Baud Rate :
     * Baud rate divisor = (UART system clock)/clkDiv*BaudRate
     * clckDiv : is determined by the HSE bit "bit 5" in UART5_CTL_REG
     *         -> 0 : clckDiv=16
     *         -> 1 : clckDiv=8
     * After doing the equation -> the answer is divided into two parts
     * Integer part    -> stored in the UART5_IBRD_REG
     * Fractional part -> stored in the UART5_FBRD_REG
     *
    */
    //Configuring the UART0 with BaudRate 9600
    UART5_IBRD_R = 104;
    UART5_FBRD_R = 11;

    /* UART Line Control Register Settings
     * send break :
     *             BRK = 0 Normal Use
     * PEN = 0 Disable Parity
     * even parity select : malhash lazma la2enak kda kda afel al Parity checker
     *                     EPS = 0 No affect as the parity is disabled
     * STP2 = 0 1-stop bit at end of the frame
     * FIFO'S enable : lw be 1 -> kda ba2a fy buffer fel FIFO ; te2dar tekhazn aktar mn byte fel register
     *                FEN = 0 FIFOs are disabled
     * word length : number of bits transmitted or received in a frame
     *              WLEN = 0x3 8-bits data frame
     * stick parity :
     *               SPS = 0 no stick parity
     */
    UART5_LCRH_R = (UART_DATA_8BITS << UART_LCRH_WLEN_BITS_POS);

    /* UART Control Register Settings
     * RXE = 1 Enable UART Receive
     * TXE = 1 Enable UART Transmit
     * HSE = 0 The UART is clocked using the system clock divided by 16
     * UARTEN = 1 Enable UART
     */
    UART5_CTL_R = UART_CTL_UARTEN_MASK | UART_CTL_TXE_MASK
            | UART_CTL_RXE_MASK;
}

void UART5_SendByte(uint8 byte)
{
    //wait until the transmit FIFO is empty -> check on 1 "it's full"
    while (UART5_FR_R & (1 << 5));
    //send the byte
    UART5_DR_R = byte;
}

uint8 UART5_RecieveByte(void)
{
    //wait until the receive FIFO is not empty -> check on 0 "can receive data"
    while (!(UART5_FR_R & (1 << 6)));
    return UART5_DR_R;
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART5_SendString(const uint8 *Str)
{
    uint32 i = 0;

    /* Send the whole string */
    while (Str[i] != '\0')
    {
        //send the byte
        UART5_SendByte(Str[i]);
        //increment the counter to the next byte
        i++;
    }
    /************************* Another Method *************************
     while(*Str != '\0')
     {
     UART0_SendByte(*Str);
     Str++;
     }
     *******************************************************************/
}

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART5_ReceiveString(uint8 *Str)
{
    uint32 i = 0;

    /* Receive the first byte */
    Str[i] = UART5_RecieveByte();

    /* Receive the whole string until the '#' */
    while (Str[i] != '#')
    {
        i++;
        Str[i] = UART5_RecieveByte();
    }

    /* After receiving the whole string plus the '#', replace the '#' with '\0' */
    Str[i] = '\0';
}

/*
 * Description :
 * Send the required number of bytes through UART to the other UART device.
 */
void UART5_SendData(const uint8 *pData, uint32 uSize)
{
    uint32 i = 0;

    /* Send required number of bytes */
    while (i <= uSize)
    {
        //send the byte
        UART5_SendByte(pData[i]);
        //increment the counter to the next byte
        i++;
    }
}

/*
 * Description :
 * receive the required number of bytes through UART from the other UART device.
 */
void UART5_ReceiveData(uint8 *pData, uint32 uSize)
{
    uint32 i = 0;

    /* Receive the first byte */
    pData[i] = UART5_RecieveByte();

    /* Receive the required number of bytes */
    while (i <= uSize)
    {
        i++;
        pData[i] = UART5_RecieveByte();
    }
}
