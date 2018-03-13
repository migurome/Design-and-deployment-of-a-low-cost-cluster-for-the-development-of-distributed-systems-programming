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
plot 'datos.txt' using 2:xtic(1) title "a", "" using 3 title "b", "" using 4 title "c", "" using 5 title "d", "" using 6 title "e", "" using 7 title "f", "" using 8 title "g"