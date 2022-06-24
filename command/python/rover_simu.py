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
turtle.speed(9)
while True:
    yaw = 0
    turtle.color('red', 'yellow')
    turtle.begin_fill()
    turtle.shapesize(4.5, 6.5, 3)

    obs = [[150,30]]
    obs_control_record = [0]
    # for i in range(6):
    #     obs.append([random.randint(-100, 100), random.randint(-100, 100)])
    #     obs_control_record.append(0)

    for ob in obs:
        turtle.penup()
        turtle.goto(ob[0], ob[1])
        turtle.pendown()
        turtle.dot(10)

    turtle.penup()
    turtle.goto(150, 100)
    turtle.pendown()

    target_pos_x = 150
    target_pos_y = -500
    obs_pos_x = 10
    obs_pos_y = 60
    turtle.speed(9)
    target_state = 0
    target_yaw = 0
    state = 0
    speed = 3

    real_target_yaw = 0

    while True:
        state += 1

        if (turtle.position()[1] - target_pos_y) ** 2 + (turtle.position()[0] - target_pos_x) ** 2 <= 10:
            # print("get to point!")
            # if target_state == 0:
            #     target_pos_x -= 25
            #     target_state += 1
            # elif target_state == 1:
            #     target_pos_y = -50
            #     target_state += 1
            # elif target_state == 2:
            #     target_pos_x -= 25
            #     target_state += 1
            # elif target_state == 3:
            #     target_pos_y = 150
            #     target_state = 0
            # if target_state == 0:
            #     target_pos_y -= 25
            #     target_state += 1
            # elif target_state == 1:
            #     target_pos_x = -150
            #     target_state += 1
            # elif target_state == 2:
            #     target_pos_y -= 25
            #     target_state += 1
            # elif target_state == 3:
            #     target_pos_x = 150
            #     target_state = 0
            if target_pos_x < -150:
                turtle.clearscreen()
                break
                turtle.done()

        c_pos = turtle.position()

        raw_target_yaw = 0
        yaw_control = 0

        yaw_control_list = []
        distance_list = []
        distance_all = 0
        obs_distance_all = 0
        # control start
        obs_id=0
        for ob in obs:
            obs_pos_x = ob[0]
            obs_pos_y = ob[1]
            ad = angle_diff(towards(obs_pos_x, obs_pos_y, c_pos[0], c_pos[1]), yaw)  # angle diff
            obs_dis = ((obs_pos_x - c_pos[0]) ** 2 + (obs_pos_y - c_pos[1]) ** 2) ** 0.5
            if abs(ad) <= 90:  # detect for available obstacle
                av = (90 - ad) * (0.5 * ad + 45) / 90  # avoiding angle

                if ad > 0:  # reverse angle
                    av = -av
                if obs_dis < 35:  # avoiding distance
                    yaw_control_list.append(av)
                    obs_factor = (35 - obs_dis) / 35
                    distance_list.append(obs_factor)
                    distance_all += obs_factor
                    obs_distance_all += obs_dis
                    obs_control_record[obs_id]=av
                turtle.pencolor("red")
            else:
                # if obs_dis < 35:
                #     yaw_control_list.append(obs_control_record[obs_id])
                #     obs_factor = (35 - obs_dis) / 35
                #     distance_list.append(obs_factor)
                #     distance_all += obs_factor
                #     obs_distance_all += obs_dis
                #     obs_control_record[obs_id]*=obs_factor
                turtle.pencolor("green")
            obs_id+=1
        for i in range(len(distance_list)):
            # distance weighted * av * distance factor * avoidance mag for final output
            raw_target_yaw += 3 * yaw_control_list[i] * distance_list[i] * distance_list[i] / distance_all

        if state % 1 == 0:
            target_yaw = turtle.towards(target_pos_x, target_pos_y)
        target_yaw += raw_target_yaw

        # FIR filtering (important!)
        real_target_yaw = 0.9 * real_target_yaw + 0.1 * target_yaw

        # turning simulation
        yaw_speed = 10
        yaw_err = real_target_yaw - yaw
        if yaw_err >= yaw_speed:
            yaw_err = yaw_speed
        elif yaw_err <= -yaw_speed:
            yaw_err = -yaw_speed
        if obs_distance_all == 0:
            obs_distance_all = math.inf
        if state % 2 == 0:
            distance_add = math.e ** (-obs_distance_all)
            print(distance_add)
            speed = 2* (-(abs(yaw_err) / yaw_speed) ** (1.5 + distance_add) + 1)
        yaw += yaw_err
        turtle.setheading(yaw)
        turtle.forward(speed)
