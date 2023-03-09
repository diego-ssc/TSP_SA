set terminal unknown
plot 'data.dat' with lines lt -1

set terminal svg dynamic size 1200,560
set yrange [0:GPVAL_DATA_Y_MAX+(GPVAL_DATA_Y_MAX*0.5)]
set output 'data.svg'
set ytics 0.2
plot 'data.dat' with lines lt -1, GPVAL_DATA_Y_MIN w p ls 0
set output
