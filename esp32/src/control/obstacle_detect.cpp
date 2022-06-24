#include "obstacle_detect.h"
#include "obstacle_avoidance.h"
#include "../driver/optical_flow.h"
#include "control.h"
#include "../comms/comms.h"

bool IsObsDetectAvailable(int c_x, int c_y, double distance_y)
{
    return abs(c_x) <= 260 && c_y <= 420 && c_y >= 50 && distance_y >= 8 && distance_y <= 70;
}

void GetRotTransform(float x, float y, float &rx, float &ry, float angle)
{
    float a = angle * MATH_PI / 180;
    rx = cos(a) * x - sin(a) * y;
    ry = sin(a) * x + cos(a) * y;
}



void ObstacleDetect(int c_x, int c_y, double distance_y, double distance_x,unsigned int color)
{
    if (IsObsDetectAvailable(c_x, c_y, distance_y))
    {   
        Serial.print("obs ok! ");
        float rx,ry;
        GetRotTransform(distance_x,distance_y,rx,ry,CurrentYaw);
        float obs_x,obs_y;
        obs_x=RealPosX + rx;
        obs_y= RealPosY + ry;
        int obs_id=AddObstacle(obs_x,obs_y);
        String msg = "$obs:";
        msg += String(obs_id);
        msg += " ";
        msg += String(obs[0][0]);
        msg += " ";
        msg += String(obs[0][1]);
        MonitorSend(msg);
    }else{
           Serial.print("obs not ok! ");
    }
}