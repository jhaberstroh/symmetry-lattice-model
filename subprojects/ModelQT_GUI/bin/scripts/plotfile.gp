set title "ordPar_R8_J-0_Q2-0_Q4-0_5.csv"
plot "../ordPar_R8_J-0_Q2-0_Q4-0_5.csv" u 0:3 w lp
call "export.gp" "../ordPar_R8_J-0_Q2-0_Q4-0_5.csv.png"
