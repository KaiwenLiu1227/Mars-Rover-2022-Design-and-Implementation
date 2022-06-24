#ifndef CONTROL_H
#include "global.h"
#include <PID_v1.h>

extern double TargetYaw, CurrentYaw, YawControl,YawDiff;
extern double TargetVelocity, CurrentVelocity, VelocityControl;
extern double  CurrentDistance, DistanceControl; //TargetDistance,
extern double TargetPosX,TargetPosY;
extern double CurrentPosX,CurrentPosY,PosXControl,PosYControl;
extern double TargetPosX,TargetPosY;
extern double TargetVelocityX,TargetVelocityY;
#define Yaw_Kp 5
#define Yaw_Ki 0
#define Yaw_Kd 0
#define Yaw_Limit 100

#define Distance_Kp 6
#define Distance_Ki 0
#define Distance_Kd 0

#define Velocity_Kp 8
#define Velocity_Ki 0
// #define Velocity_Ki 0
#define Velocity_Kd 0
#define Velocity_Limit 230

#define PosX_Kp 1.2
#define PosX_Ki 0
#define PosX_Kd 0

#define PosY_Kp 1.2
#define PosY_Ki 0
#define PosY_Kd 0

#define Pos_Limit 30

// #define PosX_Kp 1
// #define PosX_Ki 0
// #define PosX_Kd 0

// #define PosY_Kp 1
// #define PosY_Ki 0
// #define PosY_Kd 0

#define AUTO_CONTROLLER 0
#define AUTO_YAW_CONTROLLER 1
#define MANUAL_CONTROLLER 2

extern PID YawController,VelocityController,DistanceController,PosXController,PosYController;
extern int ControlMode;
void InitController();
#endif