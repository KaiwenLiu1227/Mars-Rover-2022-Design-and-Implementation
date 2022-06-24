import asyncio
import time
import turtle
from turtle import *

import pygame
import websockets

obs = []
t_obs = turtle.Turtle()
t_obs.hideturtle()
length = 50


def set_x_offset(yaw):
    cmessage = bytearray(b'$')
    print("offset:" + str(yaw))
    cmessage.append(2)
    cmessage.append(0)
    if yaw > 0:
        cmessage.append(4)
    else:
        cmessage.append(5)
    cmessage.append(abs(yaw))
    return cmessage


def add_obs(x, y):
    t_obs.clear()

    t_obs.penup()
    t_obs.speed(10)
    print(-x, y)
    t_obs.goto(-x, y)
    t_obs.pendown()
    t_obs.dot(10)
    t_obs.penup()


for ob in obs:
    turtle.penup()
    turtle.speed(10)
    turtle.hideturtle()
    turtle.goto(-ob[0], ob[1])
    turtle.pendown()
    turtle.dot(10)
turtle.penup()
turtle.goto(0, 0)
turtle.pendown()
turtle.speed(10)
color('red')
begin_fill()


async def monitor():
    async with websockets.connect(
            'ws://192.168.4.1:80/control', timeout=0.2, close_timeout=0.2) as websocket:
        pygame.init()
        pygame.joystick.init()
        joystick = pygame.joystick.Joystick(0)
        joystick.init()

        while True:
            cmessage = bytearray(b'$')
            cmessage.append(0)
            cmessage.append(0)
            await websocket.send(cmessage)
            rev = await websocket.recv()
            print(rev)
            if rev == "OK":
                break
            time.sleep(0.05)
        state = 0
        done = False

        r_state = 0
        pos_x = 0
        pos_y = 0

        ctrl = 1000
        posx = 0
        posy = 0
        start=True
        while (done != True):
            state += 1
            for event in pygame.event.get():  # User did something
                if event.type == pygame.QUIT:  # If user clicked close
                    done = True  # Flag that we are done so we exit this loop
            if not start:
                start=abs(int(joystick.get_axis(0) * 25))>=10

            x_off = -int(joystick.get_axis(2) * 25)
            if abs(x_off) >= 3:
                await websocket.send(set_x_offset(x_off))

            if start:
                if ((pos_x - posx / 2) ** 2 + (pos_y - posy / 2) ** 2) ** 0.5 <= 10 and abs(ctrl) <= 70:
                    if pos_x >= 90:
                        print("finish!")
                    elif r_state == 0:
                        pos_y = length
                    elif r_state == 1:
                        pos_x = pos_x + 30
                    elif r_state == 2:
                        pos_y = 0
                    elif r_state == 3:
                        r_state = -1
                        pos_x = pos_x + 30
                    r_state = r_state + 1
                    print(pos_x, pos_y)
                    cmessage = bytearray(b'$')
                    cmessage.append(2)
                    cmessage.append(1)
                    if pos_x > 0:
                        cmessage.append(1)
                    else:
                        cmessage.append(0)
                    cmessage.append(abs(pos_x))
                    if pos_y > 0:
                        cmessage.append(1)
                    else:
                        cmessage.append(0)
                    cmessage.append(abs(pos_y))
                    await websocket.send(cmessage)
                # else:
                print(ctrl)
                print(((pos_x - posx) ** 2 + (pos_y - posy) ** 2) ** 0.5)

            try:
                rev = await asyncio.wait_for(websocket.recv(), timeout=1)
                if "$obs" in rev:
                    print(rev)
                    data = rev.split("$obs")[-1].split(" ")
                    add_obs(float(data[1]) * 2, float(data[2]) * 2)
                elif "$opt" in rev:
                    pos = rev.split(",real:")[-1].split(",yaw")[0].split(" ")
                    ctrl = int(rev.split("ctrl:")[-1].split(",")[0])
                    yaw = (float(rev.split(",yaw:")[-1].split(",ctrl")[0]) * 180 / 3.14159) + 90
                    posx = float(pos[0]) * 2
                    posy = float(pos[1]) * 2
                    turtle.setheading(yaw)
                    turtle.goto(-posx, posy)
                # await asyncio.sleep(0.1)
            except asyncio.TimeoutError:
                print('timeout!')

                break
            print(rev)


while True:
    asyncio.get_event_loop().run_until_complete(monitor())
    # time.sleep(1)
