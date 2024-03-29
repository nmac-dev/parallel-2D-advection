#!/bin/sh

### Variables

## Compiler
CSTD="-std=c99"    # C standard
OPENMP="-fopenmp"  # Use OpenMP
CLINK="-lm"        # links library file 

## Directories
SRC_DIR="src" # source
OUT_DIR="out" # output

## Targets
TARGET="advection2D"
SRC=$SRC_DIR/$TARGET".c" # src
EXE=$OUT_DIR/$TARGET     # exe

## GNUPlot subscripts
P_INIT="xy_initial.gnuplot" # intial plot script
 P_FIN="xy_final.gnuplot"   # final  plot script
P_VAVG="v_averaged.gnuplot" # vertically averaged distribution plot script 

### Build

## Compile
echo $'\n'"Compling . . ."
(set -x; gcc $CSTD $OPENMP $SRC -o $EXE $CLINK);

## Run
echo $'\n'"Running program . . ."
cd $OUT_DIR
(set -x; ./$TARGET);

## Run GNUPlot subscripts
echo $'\n'"Plotting Data . . ."
(set -x; 
    gnuplot ../$P_INIT;
    gnuplot ../$P_FIN;
    gnuplot ../$P_VAVG;
);

## Remove files
rm *.dat $TARGET;

## Return
cd ../
echo $'\n'"SUCCESS!"
echo "See .png files inside the 'out' directory for generated plots"