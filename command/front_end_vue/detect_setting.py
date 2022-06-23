import cv2

# 定义窗口名称
winName = 'Mars Rover HSV tuner by Quix EIE'


def nothing(x):
    pass
cam_id = 0
while True:
    capture = cv2.VideoCapture(cam_id)
    cv2.namedWindow(winName)
    # 新建6个滑动条，表示颜色范围的上下边界，这里滑动条的初始化位置即为黄色的颜色范围
    cv2.createTrackbar('LowerbH', winName, 0, 180, nothing)
    cv2.createTrackbar('LowerbS', winName, 63, 255, nothing)
    cv2.createTrackbar('LowerbV', winName, 245, 255, nothing)
    cv2.createTrackbar('UpperbH', winName, 20, 180, nothing)
    cv2.createTrackbar('UpperbS', winName, 155, 255, nothing)
    cv2.createTrackbar('UpperbV', winName, 255, 255, nothing)
    while (1):
        # 函数cv2.getTrackbarPos()范围当前滑块对应的值
        lowerbH = cv2.getTrackbarPos('LowerbH', winName)
        lowerbS = cv2.getTrackbarPos('LowerbS', winName)
        lowerbV = cv2.getTrackbarPos('LowerbV', winName)
        upperbH = cv2.getTrackbarPos('UpperbH', winName)
        upperbS = cv2.getTrackbarPos('UpperbS', winName)
        upperbV = cv2.getTrackbarPos('UpperbV', winName)

        ret, frame = capture.read()
        img_hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        img_target = cv2.inRange(img_hsv, (lowerbH, lowerbS, lowerbV), (upperbH, upperbS, upperbV))
        img_specifiedColor = cv2.bitwise_and(frame, frame, mask=img_target)
        cv2.namedWindow(winName)
        cv2.imshow(winName, img_specifiedColor)
        key=cv2.waitKey(1)
        if key == ord('q'):
            break
        elif key == ord('c'):
            print(cam_id)
            cam_id += 1
            break
        elif key== ord('z'):
            cam_id -= 1
            if cam_id < 0:
                cam_id = 0
            break
    cv2.destroyAllWindows()
