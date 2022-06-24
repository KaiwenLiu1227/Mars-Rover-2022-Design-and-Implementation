import math
import random
import turtle

"""
Copyright Quix EIE Imperial College

Dynamic Obstacle Avoidance v2.1

Written&Invented by Quix, all right reserved.

"""


def sin(x):
    return math.sin(x * DEGREE_TO_RAD)


def cos(x):
    return math.cos(x * DEGREE_TO_RAD)


def rot_transfrom(x, y, a):
    a -= 90
    return cos(a) * x - sin(a) * y, sin(a) * x + cos(a) * y


def sign(x):
    if x >= 0:
        return 1
    else:
        return -1


def towards(x, y, cx, cy):
    dx = x - cx
    dy = y - cy
    if dx == 0:
        return 90 * sign(y)
    atanVal = math.atan(dy / dx) * 180 / math.pi
    if dx < 0 and dy < 0:
        return atanVal - 180
    elif dx < 0 and dy > 0:
        return atanVal + 180
    return atanVal


def angle_diff(a, b):
    d = a - b
    while d >= 360:
        d -= 360
    while d <= -360:
        d += 360
    if abs(d - 360) < abs(d):
        return d - 360
    if abs(d + 360) < abs(d):
        return d + 360
    return d


DEGREE_TO_RAD = math.pi / 180


c_pos = [0,55]
obs=[[0,40]]
yaw=0
raw_target_yaw = 0
yaw_control_list = []
distance_list = []
distance_all = 0
obs_distance_all = 0
# control start
for ob in obs:
    obs_pos_x = ob[0]
    obs_pos_y = ob[1]
    ad = angle_diff(towards(obs_pos_x, obs_pos_y, c_pos[0], c_pos[1]), yaw)  # angle diff
    if abs(ad) <= 90:  # detect for available obstacle
        av = (90 - ad) * (0.5 * ad + 45) / 90  # avoiding angle
        obs_dis = ((obs_pos_x - c_pos[0]) ** 2 + (obs_pos_y - c_pos[1]) ** 2) ** 0.5
        if ad > 0:  # reverse angle
            av = -av
        if obs_dis < 50:  # avoiding distance
            yaw_control_list.append(av)
            obs_factor = (50 - obs_dis) / 50
            distance_list.append(obs_factor)
            distance_all += obs_factor
            obs_distance_all += obs_dis

for i in range(len(distance_list)):
    # distance weighted * av * distance factor * avoidance mag for final output
    raw_target_yaw += 5 * yaw_control_list[i] * distance_list[i] * distance_list[i] / distance_all
print(raw_target_yaw)
