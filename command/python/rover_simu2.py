# import math
# import turtle
#
#
# def sin(x):
#     return math.sin(x * DEGREE_TO_RAD)
#
#
# def cos(x):
#     return math.cos(x * DEGREE_TO_RAD)
#
#
# def rot_transfrom(x, y, a):
#     print(x, y)
#     a -= 90
#     return cos(a) * x - sin(a) * y, sin(a) * x + cos(a) * y
#
#
# def angle_diff(a, b):
#     d = a - b
#     while d >= 360:
#         d -= 360
#     while d <= -360:
#         d += 360
#     if abs(d - 360) < abs(d):
#         return d - 360
#     if abs(d + 360) < abs(d):
#         return d + 360
#     return d
#
#
# DEGREE_TO_RAD = math.pi / 180
# turtle.color('red', 'yellow')
# turtle.begin_fill()
# yaw = 0
#
# obs = [[0, 75], [25, 50], [-25, 50], [25, 100], [-25, 100], [-50, 75], [50, 75]]
# obs = [[0, 75]]
# for ob in obs:
#     turtle.penup()
#     turtle.goto(ob[0], ob[1])
#     turtle.pendown()
#     turtle.dot(10)
#     turtle.penup()
#     turtle.goto(50, 0)
#     turtle.pendown()
#
# target_pos_x = 50
# target_pos_y = 150
# obs_pos_x = 10
# obs_pos_y = 60
# turtle.speed(9)
# target_state = 0
# target_yaw = 0
# state = 0
# speed = 3
#
# # turtle.shape("triangle")
# # turtle.color("red")
# # turtle.shapesize(2, 1.8, 1)
#
# while True:
#     state += 1
#     turtle.setheading(yaw)
#     if turtle.position()[1] > 150:
#         target_pos_x -= 10
#         target_state += 1
#         turtle.penup()
#         turtle.goto(target_pos_x, 0)
#         turtle.pendown()
#         if target_pos_x < -50:
#             turtle.done()
#
#     c_pos = turtle.position()
#     if state % 2 == 0:
#         target_yaw = turtle.towards(target_pos_x, target_pos_y)
#     raw_target_yaw = 0
#     yaw_control = 0
#
#     yaw_control_list = []
#     distance_list = []
#     distance_all = 0
#     for ob in obs:
#         obs_pos_x = ob[0]
#         obs_pos_y = ob[1]
#         print(angle_diff(turtle.towards(obs_pos_x, obs_pos_y), yaw))
#         if abs(angle_diff(turtle.towards(obs_pos_x, obs_pos_y), yaw)) <= 180:
#             ad = angle_diff(turtle.towards(obs_pos_x, obs_pos_y), yaw)
#             av = 4*(90 - ad) * (0.5 * ad + 45) / 90
#             obs_dis = ((obs_pos_x - c_pos[0]) ** 2 + (obs_pos_y - c_pos[1]) ** 2) ** 0.5
#             # if (turtle.towards(obs_pos_x, obs_pos_y) - yaw) > 0:
#             #     av = -av
#             if obs_dis < 50:
#                 pass
#             yaw_control_list.append(av)
#             obs_factor = (50 - obs_dis) / 50
#             distance_list.append(obs_factor)
#             distance_all += obs_factor
#
#             turtle.pencolor("red")
#         else:
#             turtle.pencolor("green")
#         # if yaw_control >= 25:
#         #     yaw_control = 25
#         # elif yaw_control <= -25:
#         #     yaw_control = -25
#         # raw_target_yaw += yaw_control
#
#         for i in range(len(distance_list)):
#             raw_target_yaw += yaw_control_list[i] *distance_list[i]* distance_list[i] / distance_all
#         print(raw_target_yaw)
#         yaw_speed = 5
#         # if raw_target_yaw >= yaw_speed:
#         #     raw_target_yaw = yaw_speed
#         # elif raw_target_yaw <= -yaw_speed:
#         #     raw_target_yaw = -yaw_speed
#         target_yaw += raw_target_yaw
#         # yaw = yaw * 0.8 + target_yaw * 0.2
#         # target_yaw =target_yaw*0.95+(target_yaw+yaw_control)*0.05
#         yaw_err = target_yaw - yaw
#
#         if yaw_err >= yaw_speed:
#             yaw_err = yaw_speed
#         elif yaw_err <= -yaw_speed:
#             yaw_err = -yaw_speed
#         if state % 2 == 0:
#             speed = 3 * (-abs(yaw_err) / yaw_speed + 1.5)
#         turtle.forward(speed)
#         yaw += yaw_err
#
# turtle.done()

import math
import turtle
import random


def sin(x):
    return math.sin(x * DEGREE_TO_RAD)


def cos(x):
    return math.cos(x * DEGREE_TO_RAD)


def rot_transfrom(x, y, a):
    print(x, y)
    a -= 90
    return cos(a) * x - sin(a) * y, sin(a) * x + cos(a) * y


DEGREE_TO_RAD = math.pi / 180

while True:
    yaw = 0
    turtle.color('red', 'yellow')
    turtle.begin_fill()
    # obs = [[0, 75], [25, 50], [-25, 50], [25, 100], [-25, 100], [-50, 75], [50, 75]]
    # obs = [[0, 75], [35, 50], [-25, 90], [25, 130], [-15, 100], [-50, 115], [50, 75]]
    # obs = [[50, 60],[-20, 30]]
    obs=[]
    for i in range(15):
        obs.append([random.randint(-100,100),random.randint(-100,100)])

    for ob in obs:
        turtle.penup()
        turtle.goto(ob[0], ob[1])
        turtle.pendown()
        turtle.dot(10)

    turtle.penup()
    turtle.goto(100, -150)
    turtle.pendown()

    target_pos_x = 100
    target_pos_y = 150
    obs_pos_x = 10
    obs_pos_y = 60
    turtle.speed(9)
    target_state = 0
    target_yaw = 0
    state = 0
    speed = 3

    # turtle.shape("triangle")
    # turtle.color("red")
    # turtle.shapesize(2, 1.8, 1)
    real_target_yaw=0
    while True:
        state += 1
        turtle.setheading(yaw)
        if (turtle.position()[1] - target_pos_y) ** 2 + (turtle.position()[0] - target_pos_x) ** 2 <= 10:
            print("finsih")
            if target_state == 0:
                target_pos_x -= 25
                target_state += 1
            elif target_state == 1:
                target_pos_y = -150
                target_state += 1
            elif target_state == 2:
                target_pos_x -= 25
                target_state += 1
            elif target_state == 3:
                target_pos_y = 150
                target_state = 0
            if target_pos_x<-100:
                turtle.clearscreen()
                break
                turtle.done()
            # turtle.done()
            # target_pos_x+=20
            # turtle.penup()
            # turtle.goto(0,0)
            # turtle.pendown()
            # while(target_pos_x>=130):
            #     turtle.done()
        c_pos = turtle.position()
        if state % 2 == 0:
            target_yaw = turtle.towards(target_pos_x, target_pos_y)
        raw_target_yaw = 0
        yaw_control = 0

        yaw_control_list = []
        distance_list = []
        distance_all = 0
        for ob in obs:
            obs_pos_x = ob[0]
            obs_pos_y = ob[1]
            if abs(turtle.towards(obs_pos_x, obs_pos_y) - yaw) <= 90:
                # av = 90 - abs(turtle.towards(obs_pos_x, obs_pos_y) - yaw)
                ad = abs(turtle.towards(obs_pos_x, obs_pos_y) - yaw)
                av = (90 - ad) * (0.5 * ad + 45) / 90
                obs_dis = ((obs_pos_x - c_pos[0]) ** 2 + (obs_pos_y - c_pos[1]) ** 2) ** 0.5
                if (turtle.towards(obs_pos_x, obs_pos_y) - yaw) > 0:
                    av = -av
                if obs_dis < 50:
                    yaw_control_list.append(av)
                    obs_factor = (50 - obs_dis) / 50
                    distance_list.append(obs_factor)
                    distance_all += obs_factor

                turtle.pencolor("red")
            else:
                turtle.pencolor("green")
            # if yaw_control >= 25:
            #     yaw_control = 25
            # elif yaw_control <= -25:
            #     yaw_control = -25
            # raw_target_yaw += yaw_control

        for i in range(len(distance_list)):
            raw_target_yaw += 1* yaw_control_list[i] * distance_list[i] * distance_list[i] / distance_all

        yaw_speed = 10
        # if raw_target_yaw >= yaw_speed:
        #     raw_target_yaw = yaw_speed
        # elif raw_target_yaw <= -yaw_speed:
        #     raw_target_yaw = -yaw_speed
        target_yaw += raw_target_yaw
        # yaw = yaw * 0.8 + target_yaw * 0.2
        # target_yaw =target_yaw*0.95+(target_yaw+yaw_control)*0.05
        real_target_yaw=0.8*real_target_yaw+0.2*target_yaw
        yaw_err = real_target_yaw - yaw

        if yaw_err >= yaw_speed:
            yaw_err = yaw_speed
        elif yaw_err <= -yaw_speed:
            yaw_err = -yaw_speed
        if state % 2 == 0:
            speed = 3 * (-(abs(yaw_err) / yaw_speed)**2 + 1)
        turtle.forward(speed)
        yaw += yaw_err

