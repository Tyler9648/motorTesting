/*****************************************
* Class:: CSC-615-01 Spring 2024
* Name:: Tyler Hsieh
* Student ID:: 920216320
* Github-Name:: Tyler9648
* Project:: Assignment 3 - Start Your Motor
*
* File:: motor.h
*
* Description:: Motor driver that controls speed,
* direction, and intialization of motors. 
*
*
******************************************/
#ifndef __MOTOR_
#define __MOTOR_

#include "motorLibrary.h"
#include <stdbool.h>

//PCA chip channels
#define PWMA        PCA_CHANNEL_0
#define AIN1        PCA_CHANNEL_1
#define AIN2        PCA_CHANNEL_2
#define PWMB        PCA_CHANNEL_5
#define BIN1        PCA_CHANNEL_3
#define BIN2        PCA_CHANNEL_4

#define MOTORA       0
#define MOTORB       1

#define SEC_TO_MICROSEC 1000000
#define CLOCKS_PER_SEC_O 100000             //clock ticks per second for Raspberry Pi
#define ZERO         0


int Motor_Init(void);
void Motor_Accelerate(uint8_t motor, int startVelocity, int endVelocity, int duration);
void Motor_setVelocity(uint8_t motor, int velocity);
#endif