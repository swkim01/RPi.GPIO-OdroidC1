import RPi.GPIO as GPIO
import time

pwm_pin = 33
GPIO.setmode(GPIO.BOARD)
GPIO.setup(pwm_pin, GPIO.OUT)
GPIO.output(pwm_pin, False)
pwm = GPIO.HWPWM(pwm_pin, 100)
angle = 3
pwm.start(3)

speed=0
while True:
    cmd = raw_input("Command. f/r: ")
    direction = cmd[0]
    if direction == 'f':
        angle += 1
    else:
        angle -= 1
    if angle < 3:
        angle = 3
    elif angle > 20:
        angle = 20
    print "angle=", (angle-3)*10
    pwm.ChangeDutyCycle(angle)
