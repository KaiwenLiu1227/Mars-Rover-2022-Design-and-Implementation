import asyncio
import time
import turtle
from turtle import *

import websockets

obs = []
t_obs = turtle.Turtle()
t_obs.hideturtle()

def add_obs(x, y):
    t_obs.clear()

    t_obs.penup()
    t_obs.speed(10)
    print(-x,y)
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
        while True:
            try:
                rev = await asyncio.wait_for(websocket.recv(), timeout=1)
                if "$obs" in rev:
                    print(rev)
                    data = rev.split("$obs")[-1].split(" ")
                    add_obs(float(data[1])* 2, float(data[2])* 2)
                elif "$opt" in rev:
                    pos = rev.split(",real:")[-1].split(",yaw")[0].split(" ")

                    yaw = (float(rev.split(",yaw:")[-1].split(",ctrl")[0]) * 180 / 3.14159) + 90
                    posx = float(pos[0]) * 2
                    posy = float(pos[1]) * 2
                    print(yaw)
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
