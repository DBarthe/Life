#!/usr/bin/python2

from random import randint
import sys
import string

if len(sys.argv) < 4:
    sys.stderr.write('Usage %s filename x y\n' % sys.argv[0])
    sys.exit(1)

file = open(sys.argv[1], "w")
size_x = string.atoi(sys.argv[2])
size_y = string.atoi(sys.argv[3])

for j in range(0, size_y):
    for i in range(0, size_x):
        c = randint(0, 3)
        if (c > 0):
            c = 0
        else:
            c = 1
        file.write('%d' % c)
    file.write("\n");

file.close();
