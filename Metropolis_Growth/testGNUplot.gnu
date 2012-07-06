set terminal x11 persist

set key left box
set samples 50
plot [][0:1.1] "ordPar_R8_J-1_Q2-3_Q4-0.csv" using 0:1 with linespoints

set terminal push
set terminal png
set output "filename.png"
replot
set output
set terminal pop