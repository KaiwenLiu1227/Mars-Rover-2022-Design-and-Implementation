#include "utils.h"

void Blink(){
    // digitalWrite(PIN_DEBUG_LED,HIGH);
    // delay(200);
    // digitalWrite(PIN_DEBUG_LED,LOW);
    // delay(200);
}

int MathSign(int i){
    if(i>=0)
        return 1;
    else
        return -1;
}
int MathSign(float i){
    if(i>=0)
        return 1;
    else
        return -1;
}

int MathSign(double i){
    if(i>=0)
        return 1;
    else
        return -1;
}

float _GetAngleDiff(float angle1,float angle2){
    float angleDiff=angle1-angle2;
    while (angleDiff >= 360)
      {
        angleDiff -= 360;
      }
      while (angleDiff <= -360)
      {
        angleDiff += 360;
      }
      if(abs(angleDiff+360)<abs(angleDiff)){
          return angleDiff+360;
      }
      if(abs(angleDiff-360)<abs(angleDiff)){
          return angleDiff-360;
      }
      return angleDiff;
}

double GetAngleDiff(double angle1,double angle2){
    double angleDiff=angle1-angle2;
    while (angleDiff >= 360)
      {
        angleDiff -= 360;
      }
      while (angleDiff <= -360)
      {
        angleDiff += 360;
      }
      if(abs(angleDiff+360)<abs(angleDiff)){
          return angleDiff+360;
      }
      if(abs(angleDiff-360)<abs(angleDiff)){
          return angleDiff-360;
      }
      return angleDiff;
}
void GetRawAvoidanceVector(double Ox,double Oy,double Rx,double Ry,float yaw,double &av_x,double &av_y){
    double ux=Ox-Rx;
    double uy=Oy-Ry;
    if(abs(ux)<=0.01)
      ux=0.01;
    if(abs(uy)<=0.01)
      uy=0.01;
    double vx=cos(yaw);
    double vy=sin(yaw);
    double lamda=ux*vx+uy*vy;
    av_x=ux-lamda*vx;
    av_y=uy-lamda*vy;
}

float GetTargetYaw(float XDiff,float YDiff){
    float atanVal;
      if (abs(XDiff) <= 0.000001)
      {
        XDiff = MathSign(XDiff)*0.000001;
      }
      if (abs(YDiff) <= 0.000001)
      {
        YDiff = MathSign(YDiff)*0.000001;
      }
      if (XDiff == 0 && YDiff == 0)
        atanVal = 0;
      else 
        atanVal = atan(YDiff / XDiff) * 180 / MATH_PI;
      if(XDiff<0&YDiff<0)
        return atanVal-180;
      else if(XDiff<0&YDiff>0)
         return atanVal+180;
      else 
        return atanVal;
}

double GetDistance(double Ox,double Oy,double Rx,double Ry){
    return sqrt(pow((Ox-Rx),2)+pow((Oy-Ry),2));
}

float _GetDistance(float Ox,float Oy,float Rx,float Ry){
    return sqrt(pow((Ox-Rx),2)+pow((Oy-Ry),2));
}

double GetTowardsAngle(double x,double y,double cx,double cy){
    double dx = x - cx;
    double dy = y - cy;
    if(dx == 0)
        return 90 * MathSign(y);
    double atanVal = atan(dy / dx) * 180 / MATH_PI;
    if (dx < 0 && dy < 0)
        return atanVal - 180;
    else if(dx < 0 && dy > 0)
        return atanVal + 180;
    return atanVal;
}

   