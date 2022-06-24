# import pyautogui as pag
#
# while True:
#     x, y = pag.position()  # 返回鼠标的坐标
#     x-=960
#     y-=540
#     x/=1920/2
#     y/=1080/2
#     posStr = "Position（x，y）:" + str(x).rjust(4) + ',' + str(y).rjust(4)
#     print(posStr)
length=20
state = 0
pos_x = 0
pos_y = 0
while True:
    if state == 0:
        pos_y = length

    elif state == 1:
        pos_x = pos_x + 10
    elif state == 2:
        pos_y = 0
    elif state == 3:
        state = -1
        pos_x = pos_x + 10
    state = state + 1
    print(state)
    print(pos_x, pos_y)
