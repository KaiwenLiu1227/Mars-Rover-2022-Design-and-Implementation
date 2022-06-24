#ifndef Utils_H
#include "global.h"
void Blink();
int MathSign(float i);
int MathSign(int i);
int MathSign(double i);
double GetAngleDiff(double angle1,double angle2);
float _GetAngleDiff(float angle1,float angle2);
void GetRawAvoidanceVector(double Ox,double Oy,double Rx,double Ry,float yaw,double &av_x,double &av_y);
float GetTargetYaw(float XDiff,float YDiff);
double GetDistance(double Ox,double Oy,double Rx,double Ry);
double GetTowardsAngle(double x,double y,double cx,double cy);
float _GetDistance(float Ox,float Oy,float Rx,float Ry);
#endif