import asyncio

import websockets

yaw_list = [90, 0, -90, 0]
top = 60
pos_list = [0, 0]
length = 60
state = 0
pos_x = 0
pos_y = 0


async def monitor():
    async with websockets.connect(
            'ws://192.168.4.1:80/control', timeout=0.2, close_timeout=0.2) as websocket:
        state = 0
        pos_x = 0
        pos_y = 0
        while True:
            if state == 0:
                pos_y = length
            elif state == 1:
                pos_x = pos_x + 30
            elif state == 2:
                pos_y = 0
            elif state == 3:
                state = -1
                pos_x = pos_x + 30
            state = state + 1
            print(pos_x,pos_y)
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
            # rev = await websocket.recv()
            # print(rev)
            await asyncio.sleep(5)
        print("finish")


while True:
    asyncio.get_event_loop().run_until_complete(monitor())
