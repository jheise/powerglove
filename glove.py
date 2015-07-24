import serial
import math
import time
import os.path

from constants import *

accel_zero = 512
scale = 100
accel_minimum = 385.0
accel_maximum = 620.0
out_minimum = -90.0
out_maximum = 90.0

thumb_full = 399.0
index_full = 302.0
middle_full = 0.0
ring_full = 331.0

thumb_zero = 462.0
index_zero = 478.0
middle_zero = 0.0
ring_zero = 523.0

FULL = "FULL"
HALF = "HALF"


def map_values(new_value, in_min=accel_minimum, in_max=accel_maximum, out_min=out_minimum, out_max=out_maximum):
    new_value = float(new_value)
    in_min = float(in_min)
    in_max = float(in_max)
    out_min = float(out_min)
    out_max = float(out_max)
    #return (new_value - in_min)*(out_max - out_min)/(in_max - in_min)+out_min
    output = (new_value - in_min)/(in_max - in_min)*(out_max - out_min)+out_min
    if output > out_max:
        output = out_max

    if output < out_min:
        output = out_min

    return output

class SerialWrapper(object):
    """
    Object to wrap around serial terminal
    """
    def __init__(self, tty="/dev/ttyACM0", baud=9600):
        self.tty = tty
        self.baud = baud
        self.serial_conn = None

    def readline(self):
        output = None

        while output == None:
            try:
                if self.serial_conn:
                    output = self.serial_conn.readline()
                else:
                    while not os.path.exists(self.tty):
                        time.sleep(.1)
                    self.serial_conn = serial.Serial(self.tty, self.baud)
            except serial.SerialException:
                if self.serial_conn:
                    self.serial_conn.close()
                del self.serial_conn
                self.serial_conn = None
                #while not os.path.exists(self.tty):
                    #time.sleep(.1)
                #self.serial_conn = serial.Serial(self.tty, self.baud)

        return output



class Glove(object):
    """
    Object to represent a power glove
    """

    def __init__(self, tty="/dev/ttyACM0", baud=9600):
        super(Glove, self).__init__()
        #self.serial_conn = serial.Serial(tty, baud)
        self.serial_conn = SerialWrapper()

        # bitmask for buttons
        self._buttons = 0

        # values for accelerometer data
        self._x = 0
        self._y = 0
        self._z = 0

        # derived values for accelerometer data
        self._xv = 0
        self._yv = 0
        self._zv = 0

        # values for bend sensors
        self._thumb = 0
        self._index = 0
        self._middle = 0
        self._ring = 0

        self.lowest = 700
        self.highest = 0

    def __str__(self):
        return "Glove T:{0} I:{1} M:{2} R:{3} X:{4} Y:{5} Z:{6} Gesture:{7}".format(
            self.thumb_flex, self.index_flex, self.middle, self.ring_flex, self.x,
            self.y, self.z, self.gesture)

    def __repr__(self):
        return "{0}(tty='{1}', baud='{2}')".format( self.__class__.__name__,
                                                    self.serial_conn.tty,
                                                    self.serial_conn.baud)

    def _get_data(self):
        output = None
        while not output:
            try:
                output = self.serial_conn.readline().strip().split("\t")
            except serial.SerialException:
                tty = self.serial_conn.port
                baud = self.serial_conn.baudrate
                self.serial_conn.close()
                self.serial_conn = serial.Serial(tty, baud)
        return output

    def update(self):
        data = self.serial_conn.readline().strip().split("\t")
        data = [float(x) for x in data]
        self._x =  1024 - data[5]
        self._y =  1024 - data[6]
        self._z = data[7]

        self._thumb = data[3]
        self._index = data[1]
        self._middle = data[2]
        self._ring = data[4]

        x_deg = map_values(self._x)
        y_deg = map_values(self._y)
        z_deg = map_values(self._z)

        x_tan = math.atan2(-y_deg, -z_deg)
        y_tan = math.atan2(-x_deg, -z_deg)
        z_tan = math.atan2(-y_deg, -x_deg)
        self._xv = math.degrees(x_tan)
        self._yv = math.degrees(y_tan)
        self._zv = math.degrees(z_tan)


    @property
    def thumb(self):
        return thumb_zero - self._thumb

    @property
    def thumb_flex(self):
        if self.thumb > 50:
            return FULL
        if self.thumb > 20:
            return HALF

    @property
    def index(self):
        return index_zero - self._index

    @property
    def index_flex(self):
        if self.index > 200:
            return FULL
        if self.index > 100:
            return HALF

    @property
    def middle(self):
        return self._middle - middle_zero

    @property
    def ring(self):
        return ring_zero - self._ring

    @property
    def ring_flex(self):
        if self.ring > 200:
            return FULL
        if self.ring > 100:
            return HALF

    @property
    def x(self):
        #return float(self._x - accel_zero)/scale
        return map_values(self._x, accel_minimum, accel_maximum, out_minimum, out_maximum)
        #return self._x

    @property
    def y(self):
        #return float(self._y - accel_zero)/scale
        return map_values(self._y, accel_minimum, accel_maximum, out_minimum, out_maximum)
        #return self._y

    @property
    def z(self):
        #return float(self._z - accel_zero)/scale
        return map_values(self._z, accel_minimum, accel_maximum, out_minimum, out_maximum)
        #return self._z

    @property
    def gesture(self):
        """
        look at state of sensors
        """

        # the horns
        if self.thumb_flex:
            if self.ring_flex:
                if self.x == 90.0:
                    return VERTHORNS
                if self.z > 75.0:
                    return FORWHORNS

        # pointing
        if self.ring_flex and not self.index_flex and not self.thumb_flex:
            if self.z > 75.0:
                return LEFTPOINT
            if self.y == 90.0:
                return RIGHTPOINT
