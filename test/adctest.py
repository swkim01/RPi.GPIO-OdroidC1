import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)
adc = GPIO.ADC(0)

while True:
    reading = adc.read()
    voltage = reading * 1.8 / 1024
    print "value=%d,\t %f V" % (reading, voltage)
    time.sleep(1)
