#ifndef DOA_H
#include "global.h"
#include "../utils/utils.h"

#define MAX_OBS_NUM 10
#define OBS_AVOIDING_RANGE 50
#define SAME_OBS_THRESHOLD 15
extern float avoiding_extent;
extern float obs[MAX_OBS_NUM][2];
extern int obs_cnt;
void UpdateObstalce(float x,float y,int id);
int AddObstacle(float x, float y);
int ForceAddObstacle(float x, float y);
float ObstacleAvoiding(float c_x,float c_y,float yaw);

#endif