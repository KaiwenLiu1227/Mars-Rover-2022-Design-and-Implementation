#ifndef MOTOR_H
#include "global.h"
#include <SparkFun_TB6612.h>
#define ENABLE_MOTOR_DEBUG 0

extern int MotorOneSpeed;
extern int MotorTwoSpeed;
extern int MotorOneControl;
extern int MotorTwoControl;
extern bool MotorOneForwarding;
extern bool MotorTwoForwarding;
extern Motor motor1;
extern Motor motor2;

void DriveMotors();

#endif