import RPi.GPIO as GPIO
import time
import os
from datetime import datetime
from telegram import Bot

botToken = '8157500884:AAHT2P1-mYzJmP_BBo0JbiS261rDkylTrTc'
chatID = '7101647082'
bot = Bot(token=botToken)
bot.send_message(chat_id = chatID, text="Telegram bot is working")

pir = 17
led = 27

GPIO.setmode(GPIO.BCM)
GPIO.setup(pir, GPIO.IN)
GPIO.setup(led, GPIO.OUT)

log_file = "camLog_v1-2.txt"
savePath = "/home/pi/Desktop/Alarm_System/motionVidsTg/"

if not os.path.exists(savePath):
    os.makedirs(savePath)

def logAndAlert(filePath, timestamp):
    log_entry = f"[{timestamp}]Motion detected, video saved: {filePath}"
    print(log_entry)
    
    with open(log_file, "a") as file:
        file.write(log_entry + "\n")

    try:
        bot.send_message(chat_id = chatID, text=f"Motion Detected at {timestamp}")

        with open(filePath, 'rb') as video:
            bot.send_video(chat_id = chatID, video = video)

    except Exception as e:
        print("Telegram send failed: ", e)

def captureVideo(duration):
    timestamp = datetime.now().strftime("%Y-%m-%d_%H:%M:%S")
    filename = f"{savePath}{timestamp}.mp4"
    cmd = f"ffmpeg -f v4l2 -framerate 30 -video_size 640x480 -t {duration} -i /dev/video0 {filename} -y -loglevel quiet"
    os.system(cmd)
    return filename, timestamp

try:
    print("System ready. Monitoring for motion...")
    
    while True:
        if GPIO.input(pir):
            GPIO.output(led, True)

            videoPath, timestamp = captureVideo(5)
            logAndAlert(videoPath, timestamp)
            time.sleep(2)
            
        else:
            GPIO.output(led, False)
        
        time.sleep(0.1)

except KeyboardInterrupt:
    print("Exiting...")

finally:
    GPIO.cleanup()
