#!/bin/sh

## Variables
C99="-std=c99"     # C standard
CLINK="-lm"        # links library file 
SRC="advection2D"  # source file
OUT_DIR="bin/"     # output directory

P_INIT="plot_initial" # intial plot script
P_FIN="plot_final"    # final  plot script

## Compile
echo $'\n'"Compling . . ."

gcc $C99 $SRC.c -o $OUT_DIR$SRC $CLINK
gcc -o bin/advection2D -std=c99 advection2D.c -lm

## Run
echo $'\n'"Running program . . ."
cd $OUT_DIR
./$SRC

## gnuplots
echo $'\n'"Plotting Initial and Final . . ."

gnuplot $P_INIT
gnuplot $P_FIN

## Return
echo "SUCCESS!"
echo "See .png files inside the 'bin' directory for intial and final plots"
cd ../