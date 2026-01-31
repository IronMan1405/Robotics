import time
from adafruit_servokit import ServoKit

kit = ServoKit(channels = 16)


while True:
    for angle in range(0, 180, 5):
        kit.servo[0].angle = angle
        print(angle)
##        time.sleep(0.1)

    print("rev")

    for angle in range(180, -1, -5):
        kit.servo[0].angle = angle
##        time.sleep(0.1)
        print(angle)
