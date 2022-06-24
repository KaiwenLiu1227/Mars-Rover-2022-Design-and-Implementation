import os

from PIL import Image
import numpy as np
import matplotlib.pyplot as plt
import cv2

# for file in os.listdir("./image"):
#     if ".jpg" not in file:
#         continue
image = Image.open("WIN_20220606_15_06_55_Pro.jpg").convert("RGB")

image = np.array(image)
image = image[:, :, ::-1]
shape = image.shape
# lower_blue = np.array([0, 0, 147])
# upper_blue = np.array([10, 224, 255])
#
# lower_blue2 = np.array([165, 0, 147])
# upper_blue2 = np.array([180, 224, 255])

lower_blue = np.array([0, 43, 46])
upper_blue = np.array([10, 255, 255])

lower_blue2 = np.array([156, 43, 46])
upper_blue2 = np.array([180, 255, 255])

# blue
lower_blue = np.array([100, 43, 46])
upper_blue = np.array([125, 255, 255])

# yellow
# lower_blue = np.array([26, 43, 46])
# upper_blue = np.array([34, 255, 255])

# cv2.imshow('Capture', frame)

# change to hsv model
hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

# get mask
mask = cv2.inRange(hsv, lower_blue, upper_blue)
mask2 = cv2.inRange(hsv, lower_blue2, upper_blue2)
# mask = cv2.bitwise_or(mask, mask2)
cv2.imshow('Mask', mask)
cv2.waitKey(0)
cv2.destroyAllWindows()
# image = cv2.bitwise_and(image, image,mask=mask)
# plt.imshow(mask)
# plt.show()
