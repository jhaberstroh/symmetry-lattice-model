set title "ordPar_R8_J-1_Q2-3_Q4-0.csv"
plot "ordPar_R8_J-1_Q2-3_Q4-0.csv" u 0:3 w lp
call "export.gp" "ordPar_R8_J-1_Q2-3_Q4-0.csv.png"
pause -1
