import asyncio

import websockets


async def monitor():
    async with websockets.connect(
            'ws://192.168.4.1:80/control', timeout=0.2, close_timeout=0.2) as websocket:
        while True:
            pos_xy = input("pos:")
            print(pos_xy)
            pos_x=int(pos_xy.split(",")[0])
            pos_y = int(pos_xy.split(",")[1])
            cmessage = bytearray(b'$')
            cmessage.append(2)
            cmessage.append(4)
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
            cmessage.append(1)
            await websocket.send(cmessage)
            rev = await websocket.recv()
            print(rev)


while True:
    try:
        asyncio.get_event_loop().run_until_complete(monitor())
    except:
        continue