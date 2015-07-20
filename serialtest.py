#!/usr/bin/env python

import serial
import atexit

ser = serial.Serial("/dev/ttyACM0", 9600)

def exit_handler():
    ser.close()

atexit.register(exit_handler)

# grab a line from the serial connection
current = ser.readline()

while current:
    print current.strip()
    current = ser.readline()

