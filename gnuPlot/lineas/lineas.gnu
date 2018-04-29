### Sen enhanced fonts
set termopt enhanced

### output for EPS
set terminal postscript enhanced color "Verdana" 
set output 'lineas.eps'

### Title and labels
set title "Detection level of t-intariants" font "Verdana, 20"
set ylabel "% of mutants detected" font "Verdana, 20"

### Ranges
set autoscale x
set yrange [0:100]

### Style and key of this chart
set grid
set style data linespoints
set key outside center bottom maxrows 1 box lt 1 lc 0 font "Verdana, 14"
#unset key

### Sizes
set ytics font "Verdana, 20"
set xtic scale 0 font "Verdana, 18"


### Plot
plot 'lineas.dat' using 2:xtic(1) title "a",\
	"" using 3 title "b",\
	"" using 4 title "c",\
	"" using 5 title "d",\
	"" using 6 title "e",\
	"" using 7 title "f",\
	"" using 8 title "g"

//Aporte de Rome, para configurar la consola de Windows

//How to fuck

set key right out
set title "Modelo 3" font "Consolas, 12"
set ylabel "Grados" font "Consolas, 10"
set xlabel "Tiempo total: 3 horas" font "Consolas, 10"

#1R
plot [10:11000][40:60] 'M1' w l lc rgb 'red'

#3R
plot [20:11000][40:60] 'morty_1' w l lc rgb 'red' ,  'morty_3' w l lc rgb 'blue',  'morty_5' w l lc rgb 'green'

#6R
plot [20:11000][40:70] 'morty_1' w l lc rgb 'red' ,  'morty_2' w l lc rgb 'blue',  'morty_3' w l lc rgb 'green',  'morty_4' w l lc rgb 'black',  'morty_5' w l lc rgb 'brown',  'morty_6' w l lc rgb 'purple'

#Estrés
set xlabel "Tiempo total: 24 horas" font "Consolas, 10"
plot [1:1400][39:55] 'Rpi 1D' w l lc rgb 'red' ,  'Rpi 2' w l lc rgb 'blue',  'Rpi 3' w l lc rgb 'green',  'Rpi 4D' w l lc rgb 'black',  'Rpi 5' w l lc rgb 'brown',  'Rpi 6' w l lc rgb 'purple'