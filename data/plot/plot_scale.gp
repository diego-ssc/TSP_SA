set terminal unknown
plot 'data.dat' with lines lt -1
set terminal svg dynamic
set output 'data_scaled.svg'
plot 'data.dat' with lines lt -1, GPVAL_DATA_Y_MIN w p ls 0
set output
