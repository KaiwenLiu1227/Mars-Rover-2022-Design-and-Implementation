#ifndef GLOBAL_H
#include "Arduino.h"

// useless pins
#define PIN_MOUSECAM_RESET     33 //A3
#define PIN_MOTOR_STBY 13 // sda


//useful pins
#define PIN_SS        5
#define PIN_MOUSECAM_CS        5
#define PIN_MISO      19
#define PIN_MOSI      23
#define PIN_SCK       18

#define PIN_MOTOR_AIN1 27 //A0 ok
#define PIN_MOTOR_BIN1 32 //A4 ok
#define PIN_MOTOR_AIN2 26 //A1 ok
#define PIN_MOTOR_BIN2 14 // 8 ok
#define PIN_MOTOR_PWMB 33// 33 //22->9 ok
#define PIN_MOTOR_PWMA 25 //A2 ok
#define MOTORA_PWM_CHANNEL 0
#define MOTORB_PWM_CHANNEL 1


// #define PIN_DEBUG_LED 14 // 10 ok
#define PIN_BEEP  13 // 21->4

#define SERIAL_USED Serial
#define MATH_PI 3.14159265358979323846

extern bool BeepControl;
#endif