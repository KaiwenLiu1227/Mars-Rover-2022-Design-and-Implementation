#ifndef MY_MPU6050_H
#include "global.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// extern Adafruit_MPU6050 mpu;
void InitMPU6050();
void MPUTest();
void UpdateMPU();
extern float MPUYawAngleIncrement,MPUYawAngle;
#endif