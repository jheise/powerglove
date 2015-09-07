#!/usr/bin/env python

import glove

power_glove = glove.Glove()

print power_glove
print power_glove.__repr__()
while True:
    power_glove.update()
    print power_glove.gesture
    #print "Thumb", power_glove._thumb, "index", power_glove._index, "ring", power_glove._ring
    #print "x raw", power_glove._x, "y raw", power_glove._y, "z raw", power_glove._z
    #print "Lowest", power_glove.lowest, "Highest", power_glove.highest
    #print "xv", power_glove._xv, "yv", power_glove._yv, "zv", power_glove._zv
    print "x", power_glove.x, "y", power_glove.y, "z", power_glove.z
