/*****************************************
* Class:: CSC-615-01 Spring 2024
* Name:: Tyler Hsieh
* Student ID:: 920216320
* Github-Name:: Tyler9648
* Project:: Assignment 3 - Start Your Motor
*
* File:: motor.c
*
* Description:: Motor driver that controls speed,
* direction, and intialization of motors. 
*
*
******************************************/
#include "motor.h"
#include <time.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

extern volatile sig_atomic_t exitProg;


/*
 * Motor intialization
 */
int Motor_Init(void)
{
    if (0 > motorLibInit()){
        printf("motorLib did not intialize properly\n");
        return -1;
    }; 
    return 0;                       //intialize with slave address on I2C bus
}                                   //higher frequency means that changing and 
                                    //maintaining motor speed is smoother

/**
 * Set motor's velocity (direction and speed)
 *
 * @param motor: select motor      // MOTORA or MOTORB
 * @param velocity: set velocity.  //(-100~100)
 *
 * Example:
 * @code
 * Motor_setVelocity(MOTORA, 80);
 * Motor_setVelocity(MOTORB, -50);
 */

void Motor_setVelocity(uint8_t motor, int velocity){

    uint8_t IN1, IN2, PWM;                //selected motor's two inputs, and PWM 

    if(motor == MOTORA){                //assign IN1, IN2, and PWM to selected motor's IN and PWM channel
        //DEBUG("Motor A selected\n");
        IN1 = AIN1;
        IN2 = AIN2;
        PWM = PWMA;
    } else {
        //DEBUG("Motor B selected\n");
        IN1 = BIN1;
        IN2 = BIN2;
        PWM = PWMB;
    }
    uint8_t speed = abs(velocity);       //speed of motors should not exceed 100%
    if (speed > 100)
        speed = 100;

    motorLib_setPWMDutyCycle(PWM, speed); //set motor's speed, is proportional to duty cycle
    if(velocity >= 0){                   //go forwards if positive
        printf("Velocity: %d\n", speed);
        motorLib_setLevel(IN1, 0);
        motorLib_setLevel(IN2, 1);    
    } else {                             //go backwards if negative 
        printf("Velocity: %d\n", -speed);
        motorLib_setLevel(IN1, 1);
        motorLib_setLevel(IN2, 0);   
    }
}
/**
 * Motor gradual acceleration 
 *
 * @param motor: Motor A or Motor B.
 * @param startVelocity: initial velocity. //(-100~100)
 * @param endVelocity: end/target velocity.  //(-100~100)
 * @param duration: duration to accelerate for in microseconds.  
 *
 * Motor_Accelerate(MOTORA, -100, 50, 2000000);
 * Motor_Accelerate(MOTORB, -100, -50, 6000000);
 */
void Motor_Accelerate(uint8_t motor, int startVelocity, int endVelocity, int duration){
    
    if(startVelocity > 100)          //absolute value of velocity should never be greater than 100%
        startVelocity = 100;         
    else if(startVelocity < -100)
        startVelocity = -100;
        
    if(endVelocity > 100)
        endVelocity = 100;
    else if(endVelocity < -100)
        endVelocity = -100;


    if(startVelocity == endVelocity || duration == 0){       //if acceleration is zero or change is instant, 
        Motor_setVelocity(motor, endVelocity);               //we set velocity and just wait out the duration
        usleep(duration);                   
    }
    else {                                                   //velocity will change over duration

        clock_t start_t, current_t, end_t, duration_t;       //used to keep time of the acceleration process
        start_t = clock();                  
        current_t = clock();                                 //current number of clock ticks

        double durationSecs = duration / SEC_TO_MICROSEC;       

        int currVelocity;                                       
        duration_t = durationSecs * CLOCKS_PER_SEC_O;                                           //convert duration from seconds to clock_t (clock ticks per sec)

        end_t = start_t + duration_t;                   

        float percentageElapsed = ((float)current_t - (float)start_t) / (float)duration_t;      //percent of duration time elapsed
 
        while(current_t <= end_t && exitProg == 0){                                                              //gradually change velocity over duration
            percentageElapsed = ((float)current_t - (float)start_t ) / (float)duration_t;       //goes from 0 to 100 as acceleration nears completion
            currVelocity = startVelocity + ( percentageElapsed * (endVelocity - startVelocity));  //velocity linearly changes with time
            Motor_setVelocity(motor, currVelocity);    
            current_t = clock();             
        }

    }

}

