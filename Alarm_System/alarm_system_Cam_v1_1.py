import RPi.GPIO as GPIO
import time
import os
from datetime import datetime

pir = 17
led = 27

GPIO.setmode(GPIO.BCM)
GPIO.setup(pir, GPIO.IN)
GPIO.setup(led, GPIO.OUT)

log_file = "camLog_v1-1.txt"
savePath = "/home/pi/Desktop/Alarm_System/motionVids/"

if not os.path.exists(savePath):
    os.makedirs(savePath)

def logMotion(filename):
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    log_entry = f"[{timestamp}]Motion detected, video saved: {filename}"
    print(log_entry)
    with open(log_file, "a") as file:
        file.write(log_entry + "\n")

def captureVideo(duration):
    timestamp = datetime.now().strftime("%Y-%m-%d_%H:%M:%S")
    filename = f"{savePath}{timestamp}.mp4"
    cmd = f"ffmpeg -f v4l2 -framerate 30 -video_size 640x480 -t {duration} -i /dev/video0 {filename} -y -loglevel quiet"
    os.system(cmd)
    return filename

try:
    print("System ready. Monitoring for motion...")
    
    while True:
        if GPIO.input(pir):
            GPIO.output(led, True)
            time.sleep(0.2)

            videoName = captureVideo(10)
            logMotion(videoName)

            time.sleep(2)
            
        else:
            GPIO.output(led, False)
        
        time.sleep(0.1)

except KeyboardInterrupt:
    print("Exiting...")

finally:
    GPIO.cleanup()
