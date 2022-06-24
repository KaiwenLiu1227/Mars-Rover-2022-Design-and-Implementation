import asyncio

import websockets

yaw_list=[90,0,-90,0]
top=20
pos_list=["0,30","25,30","25,0","30,0","50,30","75,30","75,0","100,0","100,30","125,30","125,0"]
pos_list=["0,50","50,50","50,0","0,0"]
async def monitor():
    async with websockets.connect(
            'ws://192.168.4.1:80/control', timeout=0.2, close_timeout=0.2) as websocket:
        cnt=0
        while True:
            # yaw = 0
            # cnt+=1
            # def set_add_yaw(yaw):
            #     cmessage = bytearray(b'$')
            #     cmessage.append(2)
            #     cmessage.append(0)
            #
            #     cmessage.append(4)
            #     cmessage.append(abs(yaw))
            #     return cmessage
            #
            # # rev = await websocket.recv()
            # # print(rev)
            # for y in range(6):
            #     print(y)
            #     await websocket.send(set_add_yaw(60))
            #     await asyncio.sleep(1)
            # print("finish "+str(cnt))
            # await asyncio.sleep(2)
            # continue
            #
            yaw = 0
            # def set_yaw(yaw):
            #     cmessage = bytearray(b'$')
            #     cmessage.append(2)
            #     cmessage.append(0)
            #     if yaw > 0:
            #         cmessage.append(1)
            #     else:
            #         cmessage.append(0)
            #     cmessage.append(abs(yaw))
            #     return cmessage
            # # rev = await websocket.recv()
            # # print(rev)
            # for y in yaw_list:
            #     print(y)
            #     await websocket.send(set_yaw(y))
            #     await asyncio.sleep(3)
            # print("finish")
            # # await asyncio.sleep(3)
            # continue
            for pos_ in pos_list:
                print(pos_)
                pos_xy = pos_
                pos_x=int(pos_xy.split(",")[0])
                pos_y = int(pos_xy.split(",")[1])
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
                rev = await websocket.recv()
                print(rev)
                await asyncio.sleep(10)
            print("finish")


while True:
    asyncio.get_event_loop().run_until_complete(monitor())
