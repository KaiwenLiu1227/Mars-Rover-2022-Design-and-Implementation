#include "control.h"
#include "driver/optical_flow.h"

double TargetYaw, CurrentYaw, YawControl = 0;
double TargetVelocity, CurrentVelocity, VelocityControl = 0;
double _TargetDistance, CurrentDistance, DistanceControl = 0;
double CurrentPosX, CurrentPosY, PosXControl, PosYControl = 0;
double TargetPosX, TargetPosY = 0;
double TargetVelocityX, TargetVelocityY;
double Zero, YawDiff = 0;

int ControlMode = AUTO_CONTROLLER;

PID YawController(&Zero, &YawControl, &YawDiff, Yaw_Kp, Yaw_Ki, Yaw_Kd, DIRECT);
PID VelocityController(&CurrentVelocity, &VelocityControl, &TargetVelocity, Velocity_Kp, Velocity_Ki, Velocity_Kd, DIRECT);
PID DistanceController(&Zero, &DistanceControl, &_TargetDistance, Distance_Kp, Distance_Ki, Distance_Kd, DIRECT);

PID PosXController(&CurrentPosX, &PosXControl, &TargetPosX, PosX_Kp, PosX_Ki, PosX_Kd, DIRECT);
PID PosYController(&CurrentPosY, &PosYControl, &TargetPosY, PosY_Kp, PosY_Ki, PosY_Kd, DIRECT);

void InitController()
{
    YawController.SetMode(AUTOMATIC);
    YawController.SetOutputLimits(-Yaw_Limit, Yaw_Limit);
    VelocityController.SetMode(AUTOMATIC);
    VelocityController.SetOutputLimits(-Velocity_Limit, Velocity_Limit);

    DistanceController.SetMode(AUTOMATIC);
    DistanceController.SetOutputLimits(0, 90);

    PosXController.SetMode(AUTOMATIC);
    PosXController.SetOutputLimits(-Pos_Limit, Pos_Limit);
    PosYController.SetMode(AUTOMATIC);
    PosYController.SetOutputLimits(-Pos_Limit, Pos_Limit);
}