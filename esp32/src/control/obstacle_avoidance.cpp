#include "obstacle_avoidance.h"

float obs[MAX_OBS_NUM][2] = {
    // {10, -30},{-20,-100}
};
int obs_cnt = 0;
float avoiding_extent = 4.5;

float ObstacleAvoiding(float c_x, float c_y, float yaw)
{
    bool obs_detected[MAX_OBS_NUM];
    float yaw_control_list[MAX_OBS_NUM];
    float distance_factor_list[MAX_OBS_NUM];
    float distance_factor_sum = 0;
    float obs_distance_sum = 0;
    // control start
    for (int obs_id = 0; obs_id < obs_cnt; obs_id++)
    {
        float obs_pos_x = obs[obs_id][1];
        float obs_pos_y = obs[obs_id][0];
        float ad = _GetAngleDiff(GetTowardsAngle(obs_pos_x, obs_pos_y, c_x, c_y), (float)yaw);
        if (abs(ad) <= 90)
        {                                                //detect for available obstacle
            float av = (90 - ad) * (0.5 * ad + 45) / 90; // avoiding angle
            float obs_dis = _GetDistance(obs_pos_x, obs_pos_y, c_x, c_y);
            if (ad > 0)
            { // reverse angle
                av = -av;
            }
            if (obs_dis < OBS_AVOIDING_RANGE)
            {
                obs_detected[obs_id] = true;
                yaw_control_list[obs_id] = av;
                // if(abs(obs_dis)<=20){
                //     obs_dis=MathSign(obs_dis)*(20-10*pow(2.718,-obs_dis/10));
                // }
                float obs_factor = (OBS_AVOIDING_RANGE - obs_dis) / OBS_AVOIDING_RANGE;
                distance_factor_list[obs_id] = obs_factor;
                distance_factor_sum += obs_factor;
                obs_distance_sum += obs_dis;
            }
            else
            {
                obs_detected[obs_id] = false;
            }
        }
        else
        {
            obs_detected[obs_id] = false;
        }
    }
    float raw_target_yaw = 0;
    for (int obs_id = 0; obs_id < obs_cnt; obs_id++)
    {
        if (!obs_detected[obs_id])
            continue;
        raw_target_yaw += avoiding_extent * yaw_control_list[obs_id] * distance_factor_list[obs_id] * distance_factor_list[obs_id] / distance_factor_sum;
    }
    if(raw_target_yaw>=80){
        return 80;
    }
    if(raw_target_yaw<=-80){
        return -80;
    }
    return raw_target_yaw;
}
int ForceAddObstacle(float x, float y)
{
    Serial.println("add obs!");
    if(obs_cnt+1>MAX_OBS_NUM-1)
        return -1;
    BeepControl=true;
    obs_cnt++;
    obs[obs_cnt - 1][0] = x;
    obs[obs_cnt - 1][1] = y;
    return obs_cnt-1;
}
int AddObstacle(float x, float y)
{
    for (int obs_id = 0; obs_id < obs_cnt; obs_id++)
    {
        float obs_pos_x = obs[obs_id][0];
        float obs_pos_y = obs[obs_id][1];
        float obs_dis = _GetDistance(obs_pos_x, obs_pos_y, x, y);
        if(obs_dis<=SAME_OBS_THRESHOLD){
            UpdateObstalce((obs_pos_x+x)/2,(obs_pos_y+y)/2,obs_dis);
            return obs_dis;
        }
    }
    return ForceAddObstacle(x,y);
}



void UpdateObstalce(float x, float y, int id)
{
    obs[id][0] = x;
    obs[id][1] = y;
}