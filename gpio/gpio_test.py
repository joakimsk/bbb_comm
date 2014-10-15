#!/usr/bin/env python
import Adafruit_BBIO.GPIO as GPIO

print GPIO.setup("GPIO1_7", GPIO.OUT)
print GPIO.output("GPIO1_7", GPIO.LOW)
print GPIO.cleanup()
