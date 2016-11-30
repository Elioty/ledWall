#! /usr/bin/python2

import math

steps = 64
for i in range(steps):
	val = int((math.sin(2 * i * math.pi / steps - math.pi / 2) + 1) * 2048)
	if val == 4096:
		val = 4095
	print("%d," % val)
