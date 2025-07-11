import RPi.GPIO as GPIO
import time
import os
from datetime import datetime

pir = 17
led = 27

GPIO.setmode(GPIO.BCM)
GPIO.setup(pir, GPIO.IN)
GPIO.setup(led, GPIO.OUT)

log_file = "camLog.txt"
savePath = "/home/pi/Desktop/Alarm_System/motionImgs/"

if not os.path.exists(savePath):
    os.makedirs(savePath)

def logMotion(filename):
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    log_entry = f"[{timestamp}]Motion detected, image saved: {filename}"
    print(log_entry)
    with open(log_file, "a") as file:
        file.write(log_entry + "\n")

def captureImg():
    timestamp = datetime.now().strftime("%Y-%m-%d_%H:%M:%S")
    filename = f"{savePath}{timestamp}.jpg"
    os.system(f"fswebcam -r 1280x720 --no-banner {filename}")
    return filename

try:
    print("System ready. Monitoring for motion...")
    
    while True:
        if GPIO.input(pir):
            GPIO.output(led, True)
            time.sleep(0.2)

            filename = captureImg()
            logMotion(filename)

            time.sleep(1)
            
        else:
            GPIO.output(led, False)
        
        time.sleep(0.1)

except KeyboardInterrupt:
    print("Exiting...")

finally:
    GPIO.cleanup()
