/*****************************************
* Class:: CSC-615-01 Spring 2024
* Name:: Tyler Hsieh
* Student ID:: 920216320
* Github-Name:: Tyler9648
* Project:: Assignment 3 - Start your motors
*
* File:: main.c
*
* Description:: For this program, I had to initialize it
* to a motor and run a set sequence when a button is 
* pressed
******************************************/



#include "main.h"
#include <pigpio.h>

volatile sig_atomic_t exitProg;

void Handler(int signo)                     //handles program interrupt, ex: ctrl c 
{                                           //so that motors are properly stopped
    exitProg = 1;
}

void kms(void)                     
{                                          
    printf("\r\nInterrupt handler: Stop motor\r\n");
    Motor_setVelocity(MOTORA, ZERO);       
    Motor_setVelocity(MOTORB, ZERO);   
    motorLib_terminate();
    gpioTerminate();
    exit(0);
}


void waitUntilSignal(int pin){          //waits until button is pressed and released

    while(gpioRead(pin) != 1 && exitProg = 0){}           //waiting for button push down
    
    while(gpioRead(pin) == 1 && exitProg = 0){}           //waiting for button to be released

    
    printf("\nbutton pressed!\n\n");
}

int main(void)
{
    exitProg = 0;                                   //exit when its 1

    gpioInitialise();
    if (Motor_Init() == -1){
        printf("motor lib got fucked, remove before pushing to main repo\n");
        exit(0);
    }                                    //Motor Initialization
    signal(SIGINT, Handler);

    gpioSetMode(BUTTON_GPIO, PI_INPUT);                  //Button Initialization

    waitUntilSignal(BUTTON_GPIO);                    //wait for button to be pressed before continuing

    printf("Start Motor\r\n");
    Motor_Accelerate(MOTORA, 100, 100, 2500000);      //full speed forward for 2.5 seconds
    Motor_Accelerate(MOTORA, 100, 15, 5000000);       //slow down to 15% speed over 5 seconds
    Motor_Accelerate(MOTORA, 0, 0, 1500000);          //stop for 1.5 seconds
    Motor_Accelerate(MOTORA, 0, -100, 5000000);       //full speed backwards for 5 seconds

    // Exception handling:ctrl + c

    while(exitProg == 0){}                                      //only exits upon program interrupt
    kms();
    //System Exit
    return 0;
}



