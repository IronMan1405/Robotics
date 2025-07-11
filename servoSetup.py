import time
from adafruit_servokit import ServoKit

kit = ServoKit(channels = 16)

kit.servo[4].set_pulse_width_range(600, 200)

##for i in range(5, -1, -1):
##    kit.servo[i].angle = 20
##    print(f"set servo {i+1} angle to 20")   

while True:
    kit.servo[4].angle = 150
    time.sleep(0.5)
    kit.servo[4].angle = 50
