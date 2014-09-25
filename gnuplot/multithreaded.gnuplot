# This gnuplot scipt plots the output of the multithreaded performance numbers
# from the Google spreadhseet exported to CSV.

# Output.
set terminal svg size 1200 800
set output "/tmp/zpar.svg"

# Multiplot.
set multiplot layout 2,2 title "Multithreaded, feature hashing, +0K"

# Plot 0,0
set xlabel "time (s)"
set ylabel "UAS (%)"
set datafile separator ","
plot "/tmp/zpar.csv" every ::1 using 5:2 title "1" with linespoints pointtype 7 pointsize 0.4 linetype rgb "blue", \
     "/tmp/zpar.csv" every ::1 using 9:6 title "2" with linespoints pointtype 7 pointsize 0.4 linetype rgb "red", \
     "/tmp/zpar.csv" every ::1 using 13:10 title "4" with linespoints pointtype 7 pointsize 0.4 linetype rgb "orange", \
     "/tmp/zpar.csv" every ::1 using 17:14 title "16" with linespoints pointtype 7 pointsize 0.4 linetype rgb "purple"

# Plot 1,0.
set xlabel "time (s)"
set ylabel "LAS (%)"
set datafile separator ","
plot "/tmp/zpar.csv" every ::1 using 5:3 title "1" with linespoints pointtype 8 pointsize 0.4 linetype rgb "blue", \
     "/tmp/zpar.csv" every ::1 using 9:7 title "2" with linespoints pointtype 8 pointsize 0.4 linetype rgb "red", \
     "/tmp/zpar.csv" every ::1 using 13:11 title "4" with linespoints pointtype 8 pointsize 0.4 linetype rgb "orange", \
     "/tmp/zpar.csv" every ::1 using 17:15 title "16" with linespoints pointtype 8 pointsize 0.4 linetype rgb "purple"

# Plot 0,1.
set xlabel "iteration #"
set ylabel "UAS (%)"
set datafile separator ","
plot "/tmp/zpar.csv" every ::1 using 1:2 title "1" with linespoints pointtype 7 pointsize 0.4 linetype rgb "blue", \
     "/tmp/zpar.csv" every ::1 using 1:6 title "2" with linespoints pointtype 7 pointsize 0.4 linetype rgb "red", \
     "/tmp/zpar.csv" every ::1 using 1:10 title "4" with linespoints pointtype 7 pointsize 0.4 linetype rgb "orange", \
     "/tmp/zpar.csv" every ::1 using 1:14 title "16" with linespoints pointtype 7 pointsize 0.4 linetype rgb "purple"

# Plot 1,1.
set xlabel "iteration #"
set ylabel "LAS (%)"
set datafile separator ","
plot "/tmp/zpar.csv" every ::1 using 1:3 title "1" with linespoints pointtype 8 pointsize 0.4 linetype rgb "blue", \
     "/tmp/zpar.csv" every ::1 using 1:7 title "2" with linespoints pointtype 8 pointsize 0.4 linetype rgb "red", \
     "/tmp/zpar.csv" every ::1 using 1:11 title "4" with linespoints pointtype 8 pointsize 0.4 linetype rgb "orange", \
     "/tmp/zpar.csv" every ::1 using 1:15 title "16" with linespoints pointtype 8 pointsize 0.4 linetype rgb "purple"
