#!/bin/sh

gcc -c port_range.c
gcc -c gports.c
gcc port_range.o gports.o -o port_rg 

