

import pyautogui as pag
import math

import pygame
import asyncio
import websockets
import time


def sign(x):
    if x < 0:
        return -1
    else:
        return 1


def clamp(x, max, min):
    if x <= min:
        return min
    elif x >= max:
        return max
    return x




async def control():
    async with websockets.connect(
            'ws://192.168.4.1:80/control') as websocket:
        status_byte = int(0)
        pwn1 = 200
        pwn2 = 150
        done = False
        control_period = 0.1
        while (done != True):
            print('================')
            x, y = pag.position()  # 返回鼠标的坐标
            if x < 5 and y < 5:
                cmessage = bytearray(b'$')
                cmessage.append(1)
                cmessage.append(status_byte)
                cmessage.append(0)
                cmessage.append(0)
                await websocket.send(cmessage)
                break
            x -= 960
            y -= 540
            x /= 1920 / 2
            y /= 1080 / 2
            x2=int(100*x)
            y2=int(100*y)

            print(x2,y2)
            if abs(x2) <= 0.01:
                x2 = sign(x2) * 0.01
            mag2 = (y2 ** 2 + x2 ** 2) ** 0.5
            if mag2 <= 20:
                left = 0
                right = 0
                print("brake")
            else:
                angle2 = 180 / math.pi * math.atan(abs(y2) / x2)
                if angle2 <= 0:
                    angle2 += 180
                if y2 < -10:
                    status_byte = 0
                    right = clamp(int(255 * angle2 / 90), 255, 30)
                    left = clamp(int(255 * (2 - angle2 / 90)), 255, 30)
                else:
                    status_byte = 3
                    right = clamp(int(255 * angle2 / 90), 255, 30)
                    left = clamp(int(255 * (2 - angle2 / 90)), 255, 30)
                print("%f %f" % (left, right))

            pwn1 = right
            pwn2 = left

            cmessage = bytearray(b'$')
            cmessage.append(1)
            cmessage.append(status_byte)
            cmessage.append(pwn1)
            cmessage.append(pwn2)
            await websocket.send(cmessage)
            try:
                rev = await asyncio.wait_for(websocket.recv(), timeout=0.5)
                print(rev)
            except asyncio.TimeoutError:
                print('timeout 1!')
                break
                return
            # time.sleep(control_period)
            await asyncio.sleep(0.2)
time.sleep(1)
asyncio.get_event_loop().run_until_complete(control())
# time.sleep(1)
# while True:
#
#     print("reconnect")

