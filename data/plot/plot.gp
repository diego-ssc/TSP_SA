set terminal unknown
plot 'data_2878.dat' with lines lt -1
set output
set terminal svg dynamic enhanced size 600,280
set yrange [0:GPVAL_DATA_Y_MAX+(GPVAL_DATA_Y_MAX*0.5)]
set output 'results_2878.svg'
# set ytics 0.2
unset label
min =GPVAL_DATA_Y_MIN
data = 'data_2878.dat'
plot data with lines lt -1, min w p  ls 0
set output
