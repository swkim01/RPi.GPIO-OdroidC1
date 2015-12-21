# RPi.GPIO-OdroidC1

This is a RPi.GPIO module for Odroid C1 board.

- GPIO input and output
- GPIO interrupts (callbacks when events occur on input gpios)
- Hardware/Software PWM
- Hardware ADC

Install this package by executing:
````
sudo python setup.py install
````

# Examples

#### [Hardware PWM](http://odroid.com/dokuwiki/doku.php?id=en:c1_hardware_pwm)

Odroid-C1 board has two PWM pins, #33 and #19.<br/>
To use hardware PWM, you have to load kernel PWM module.

````
sudo modprobe pwm-meson npwm=1 # or npwm=2
sudo modprobe pwm-ctrl
````

`RPi.GPIO.HWPWM` provides hardware PWM for Odroid-C1.<br/>
Example of using hardware PWM is as follows:
````
import RPi.GPIO as GPIO
import time

pwm_pin = 33
GPIO.setmode(GPIO.BOARD)
GPIO.setup(pwm_pin, GPIO.OUT)
GPIO.output(pwm_pin, False)
pwm = GPIO.HWPWM(pwm_pin, 100)
pwm.start(0)
time.sleep(1)
pwm.ChangeDutyCycle(50)
time.sleep(1)
pwm.ChangeDutyCycle(100)
````

#### [ADC](http://odroid.com/dokuwiki/doku.php?id=en:c1_hardware_adc)

Odroid-C1 board has two ADC pins, 0(#40) and 1(#39).<br/>
`RPi.GPIO.ADC` provides ADC function for Odroid-C1.<br/>
Example of using ADC is as follows:
````
import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)
adc = GPIO.ADC(0)
while True:
    reading = adc.read()
    voltage = reading * 1.8 / 1024
    print "value=%d,\t %f V" % (reading, voltage)
    time.sleep(1)
````
