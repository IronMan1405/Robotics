import RPi.GPIO as GPIO
import time
from datetime import datetime

pir = 17
led = 27

GPIO.setmode(GPIO.BCM)
GPIO.setup(pir, GPIO.IN)
GPIO.setup(led, GPIO.OUT)

log_file = "motion_log.txt"

def logMotion():
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    log_entry = f"Motion detected @ {timestamp}"
    print(log_entry)
    with open(log_file, "a") as file:
        file.write(log_entry + "\n")
        
try:
    print("System ready. Monitoring for motion...")
    
    while True:
        if GPIO.input(pir):
            GPIO.output(led, True)
            logMotion()
            time.sleep(0.5)
        else:
            GPIO.output(led, False)
        
        time.sleep(0.1)

except KeyboardInterrupt:
    print("Exiting...")

finally:
    GPIO.cleanup()
