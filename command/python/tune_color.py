import intel_jtag_uart
import ast, time
import sys
import ctypes
import matplotlib.pyplot as plt
import struct
import cv2
winName = 'Mars Rover HSV tuner by Quix EIE'

try:
    ju = intel_jtag_uart.intel_jtag_uart()

except Exception as e:
    print(e)
    sys.exit(0)


def nothing(x):
    pass


# ju.write(b'123')
# a=b'\x49\x03b\x03c\x03'
# print(a.decode())
# color_code 3
# up low 1
# 000 3
# 9hash()
# 8s
# 8v

# parameter BLACK_CODE = 3'b000;
# parameter WHITE_CODE = 3'b001;
# parameter RED_CODE = 3'b010;
# parameter BLUE_CODE = 3'b011;
# parameter YELLOW_CODE = 3'b100;
# parameter PURPLE_CODE = 3'b101;
# parameter DGREEN_CODE = 3'b110;
# parameter LGREEN_CODE = 3'b111;
# parameter UNKNOWN_CODE = 3'b000;
cam_id = 0
# color = int(input("color:")) & 0x7
winName = 'Mars Rover HSV tuner by Quix EIE'
# capture = cv2.VideoCapture(cam_id)
cv2.namedWindow(winName,cv2.WINDOW_NORMAL)
cv2.resizeWindow(winName, 300, 300)
# 新建6个滑动条，表示颜色范围的上下边界，这里滑动条的初始化位置即为黄色的颜色范围
cv2.createTrackbar('Color', winName, 0, 7, nothing)
cv2.createTrackbar('Setting Mode', winName, 0, 1, nothing)
cv2.createTrackbar('LowerbH', winName, 112, 360, nothing)
cv2.createTrackbar('LowerbS', winName, 88, 255, nothing)
cv2.createTrackbar('LowerbV', winName, 0, 255, nothing)
cv2.createTrackbar('UpperbH', winName, 176, 360, nothing)
cv2.createTrackbar('UpperbS', winName, 255, 255, nothing)
cv2.createTrackbar('UpperbV', winName, 166, 255, nothing)
# cv2.createButton("Set",nothing,buttonType=0)
state=0
while True:
    # 函数cv2.getTrackbarPos()范围当前滑块对应的值
    color= cv2.getTrackbarPos('Color', winName) & 0x7
    mode = cv2.getTrackbarPos('Setting Mode', winName)
    lowerbH = cv2.getTrackbarPos('LowerbH', winName)
    lowerbS = cv2.getTrackbarPos('LowerbS', winName)
    lowerbV = cv2.getTrackbarPos('LowerbV', winName)
    upperbH = cv2.getTrackbarPos('UpperbH', winName)
    upperbS = cv2.getTrackbarPos('UpperbS', winName)
    upperbV = cv2.getTrackbarPos('UpperbV', winName)
    if cv2.waitKey(10) & 0xFF == ord('q'):
        break
    if mode==0:
        continue
    # ret, frame = capture.read()
    # cv2.namedWindow(winName)
    # cv2.imshow(winName, frame)
    state+=1
    if state%8==0:
        h = (lowerbH) & 0x1ff
        s = lowerbS & 0xff
        v = lowerbV & 0xff
        command = (v + (s << 8) + (h << 16) + (color << 29)) & 0xffffffff

        print("lower: ", "$".encode() + command.to_bytes(4, 'big') + "#".encode())
        ju.write("$".encode() + command.to_bytes(4, 'big') + "#".encode())
        a = ju.read()
        if a:
            print("msg: ")
            print(a)
    if (state+4) % 16 == 0:
        h = (upperbH) & 0x1ff
        s = upperbS & 0xff
        v = upperbV & 0xff
        command = (v + (s << 8) + (h << 16) + (1 << 28) + (color << 29)) & 0xffffffff

        print("upper", "$".encode() + command.to_bytes(4, 'big') + "#".encode())
        ju.write("$".encode() + command.to_bytes(4, 'big') + "#".encode())
        a = ju.read()
        if a:
            print("msg: ")
            print(a)




