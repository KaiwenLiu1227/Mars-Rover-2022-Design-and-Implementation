import math

DEGREE_TO_RAD = math.pi / 180


def sin(x):
    return math.sin(x * DEGREE_TO_RAD)


def cos(x):
    return math.cos(x * DEGREE_TO_RAD)


def rot_transfrom(x, y, a):
    return cos(a) * x - sin(a) * y, sin(a) * x + cos(a) * y

print(rot_transfrom(1,1,0))