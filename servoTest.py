import time
from adafruit_servokit import ServoKit

kit = ServoKit(channels = 16)

for angle in range(0, 101, 10):
    kit.servo[3].angle = angle
    time.sleep(0.1)

for angle in range(100, -1, -10):
    kit.servo[3].angle = angle
    time.sleep(0.1)
