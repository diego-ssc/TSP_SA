#!/usr/local/bin/gnuplot --persist

set terminal unknown
plot ARG1 with lines lt -1
set output
set terminal svg dynamic enhanced size 600,280
set yrange [0:GPVAL_DATA_Y_MAX+(GPVAL_DATA_Y_MAX*0.5)]
set output ARG2
# set ytics 0.2
unset label
min =GPVAL_DATA_Y_MIN
data = ARG1
