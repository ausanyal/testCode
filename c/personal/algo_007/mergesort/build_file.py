#!/usr/bin/python
import sys

f = open("test_file", "w");

num = 100000
for i in range (1, num):
    f.write("%d\n" % (num-i))
