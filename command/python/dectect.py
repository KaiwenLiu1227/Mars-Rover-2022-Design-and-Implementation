import cv2
import numpy as np
import cv2
import numpy as np

capture = cv2.VideoCapture(0)
def rgb_to_hsv2(b, g, r):
    # r, g, b = r / 255.0, g / 255.0, b / 255.0
    mx = max(r, g, b) # /255
    mn = min(r, g, b) # /255
    df = mx - mn # /255
    h = 0
    if mx == mn:
        h = 0
    elif mx == r:
        h = int((30 * ((g - b) / df) + 180)*255) % 360
    elif mx == g:
        h = int((30 * ((b - r) / df) + 60)*255) % 360
    elif mx == b:
        h = int((30 * ((r - g) / df) + 120)*255) % 360
    if mx == 0:
        s = 0
    else:
        s = (df / mx) * 255
    v = mx
    return h, s, v


def rgb_to_hsv(b,g,r):
    r, g, b = r / 255.0, g / 255.0, b / 255.0
    mx = max(r, g, b)
    mn = min(r, g, b)
    df = mx - mn
    h = 0
    if mx == mn:
        h = 0
    elif mx == r:
        h = (60 * ((g - b) / df) + 360) % 360
    elif mx == g:
        h = (60 * ((b - r) / df) + 120) % 360
    elif mx == b:
        h = (60 * ((r - g) / df) + 240) % 360
    if mx == 0:
        s = 0
    else:
        s = (df / mx) * 255
    v = mx * 255
    return h, s, v


while (True):
    # 获取一帧
    ret, frame = capture.read()
    # 将这帧转换为灰度图

    # set red thresh
    # lower_blue=np.array([156,43,46])
    # upper_blue=np.array([180,255,255])

    # lower_blue = np.array([0, 43, 46])
    # upper_blue = np.array([10, 255, 255])

    lower_blue = np.array([100, 43, 147])
    upper_blue = np.array([125, 224, 255])

    lower_blue2 = np.array([175, 43, 147])
    upper_blue2 = np.array([180, 224, 255])

    lower_blue = np.array([200, 43, 47])
    upper_blue = np.array([250, 224, 255])
    # cv2.imshow('Capture', frame)

    # change to hsv model

    hsv2 = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    hsv=frame.copy()
    frame2 = np.array(frame)
    shape = frame2.shape
    for i in range(shape[0]):
        for j in range(shape[1]):
            h, s, v = rgb_to_hsv(frame2[i][j][0], frame2[i][j][1], frame2[i][j][2])
            hsv[i][j][0] = h
            hsv[i][j][1] = s
            hsv[i][j][2] = v
    # get mask
    mask = cv2.inRange(hsv, lower_blue, upper_blue)
    mask2 = cv2.inRange(hsv, lower_blue2, upper_blue2)
    # mask = cv2.bitwise_or(mask, mask2)
    # cv2.imshow('Mask', mask)

    # detect red

    # cv2.imshow('Result', res)

    # cv2.waitKey(0)
    # cv2.destroyAllWindows()

    # image = np.array(frame)
    # shape = image.shape
    # for i in range(shape[0]):
    #     for j in range(shape[1]):
    #         if image[i][j][0] >= 68 and image[i][j][1] <= 31 and image[i][j][2] <= 22:
    #             image[i][j][0] = 255
    #             image[i][j][1] = 0
    #             image[i][j][2] = 0
    res = cv2.bitwise_and(frame, frame, mask=mask)
    # cv2.imshow('frame', hsv)
    # cv2.imshow('frame2', hsv2)
    cv2.imshow('frame3', res)
    # if cv2.waitKey(1) == ord('q'):
    #     break
    # _input = input("enter:")
    #
    # if _input=="":
    #     continue
    # data = _input.split(":")
    # data_value = data[0].split(",")
    # if int(data[0]) == 1:
    #     lower_blue[0] = data_value[0]
    #     lower_blue[1] = data_value[1]
    #     lower_blue[2] = data_value[2]
    #
    # if int(data[0]) == 2:
    #     upper_blue[0] = data_value[0]
    #     upper_blue[1] = data_value[1]
    #     upper_blue[2] = data_value[2]
    # print("success!")
    if cv2.waitKey(1) == ord('q'):
        break
