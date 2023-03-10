set terminal unknown
plot 'data_2.dat' with lines lt -1
set term svg dynamic enhanced
set output 'data_scaled_2.svg'
min =GPVAL_DATA_Y_MIN
plot 'data_2.dat' with lines lt -1, min w p  ls 0
set output
