#!/usr/local/bin/gnuplot --persist

set terminal unknown
plot ARG1 with lines lt -1
set term svg dynamic enhanced
set output ARG2
min =GPVAL_DATA_Y_MIN
plot ARG1 with lines lt -1, min w p  ls 0
