#!/bin/sh

gcc -g -c port_range.c
gcc -g -c gports.c
gcc -g port_range.o gports.o -o port_rg -lm

#~ravir/bin/gcc -c port_range.c
#~ravir/bin/gcc -c gports.c
#~ravir/bin/gcc port_range.o gports.o -o port_rg 

