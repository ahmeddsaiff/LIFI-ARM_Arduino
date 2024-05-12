#include "main.h"

//global variable for systick
extern volatile uint8 g_counter = 0;


void SystickHandler(void){
    g_counter++;
}


int main(void)
{
    //local variable for storing the distance returned from the UltraSonic
    uint32 distance;

    /* Enable clock for PORTA , PORTB , PORTD & PORTF and wait for clock to start */
    SYSCTL_RCGCGPIO_R |= 0x2B;
    while (!(SYSCTL_PRGPIO_R & 0x2B));

    Disable_Exceptions();
    Enable_Exceptions();

    StartButton_Init();
    StopButton_Init();

    InfraTransmitter_Init();

    GasSensor_Init();
    MagneticSensor_Init();
    UltraSonic_init();

    //for Bluetooth
    UART5_Init();

    while (1)
    {
        //this flag is for start and stop buttons
        //lw dosna stop al flag -> hayeb2a be zero
        //fa hanefdal wa2feen fy awl al while loop dy mestanyeen al start button yedas
        flag = 1;

        //checking on the switch
        if (Get_StartButton_Status())
        {
            //delay for De-bouncing
            delayMs(10);

            if (Get_StartButton_Status())
            {
                //Lama tedous start button w nehs8l al system
                //haneb3at al data 3lshan n3rd 3al lcd "All is okay" w kman n3rd 3ala al mobile nafs al kalam
                //fa hanshh8l al transmitter le modet nos sanya (500ms) w ne2felo tany 3lshan neb3at al data
                EnableTransmitter();

//                delayMs(500);
                SysTick_Init();
                while(g_counter != 1);
                g_counter = 0;
                SystickTickDisable();

                DisableTransmitter();

                //send to mobile through uart5
                UART5_SendString("All is OKAY.. ");

                while (flag)
                {
                    //get the measured distance
                    distance = Measure_distance();

                    //gas sensor
                    if (GasSensor_GetStatus())
                    {
                        //hansh8l al transmitter for 1 sec w ne2felo tany 3lshan neb3at al data
                        EnableTransmitter();
//                        delayMs(1000);

                        SysTick_Init();
                        while(g_counter != 2);
                        g_counter = 0;
                        SystickTickDisable();

                        DisableTransmitter();

                        //w ba3atna lel mobile al msg
                        UART5_SendString("Fire Detected!! ");

                        //polling until the fume sensor finish reading
                        while (GasSensor_GetStatus());
//                            GPIO_PORTD_DATA_REG |= (1<<3);

                    }

                    //Magnetic switch
                    if (Magnetic_GetStatus())
                    {
                        //hansh8l al transmitter for 2 secs w ne2felo tany 3lshan neb3at al data
                        EnableTransmitter();
//                        delayMs(3000);

                        SysTick_Init();
                        while(g_counter != 4);
                        g_counter = 0;
                        SystickTickDisable();

                        DisableTransmitter();

                        //w ba3atna lel mobile al msg
                        UART5_SendString("Intrusion Detected!! ");

                        //polling until the magnetic switch sensor finish reading
                        //3lshan lw mesekna al magnet w fdelna msbtyno 3ala al sensor -> myo3odsh ye2raa w yeb3at kol shwyaa
                        while (Magnetic_GetStatus());
                    }

                    //Door is unlocked
                    if(distance < 10)
                    {
                        //hansh8l al transmitter for 1.5 secs w ne2felo tany 3lshan neb3at al data
                        EnableTransmitter();
//                        delayMs(2000);

                        SysTick_Init();
                        while(g_counter != 3);
                        g_counter = 0;
                        SystickTickDisable();

                        DisableTransmitter();

                        //w ba3atna lel mobile al msg
                        UART5_SendString("Door is unlocked!! ");

                        //polling hena 3lshan lw al sensor fedel odamo al haga 3ala masafa a2al mn 10
                        //mayfdalsh y3eed al code tany w yo3od yeb3at data w yeb3at lel mobile nafs al message kteer
                          //khly balakkkk ->
                          //                ma3mltsh al condition hena be variable al distance
                          //                3lshan lw heya be variable al distance hnefdal fel loop dy msh han5rog menaha abadn...
                        while (Measure_distance() < 10);
                    }
                }

            }

        }
    }
}


void delayMs(uint32 ui32Ms)
{

    // 1 clock cycle = 1 / SysCtlClockGet() second
    // 1 SysCtlDelay = 3 clock cycle = 3 / SysCtlClockGet() second
    // 1 second = SysCtlClockGet() / 3
    // 0.001 second = 1 ms = SysCtlClockGet() / 3 / 1000

    SysCtlDelay(ui32Ms * (SysCtlClockGet() / 3 / 1000));
}
