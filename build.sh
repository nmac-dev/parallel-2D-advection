#!/bin/sh

### Variables

## Compiler
CSTD="-std=c99"     # C standard
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

### Build

## Compile
echo $'\n'"Compling . . ."
(set -x; gcc $CSTD $SRC -o $EXE $CLINK);

## Run
echo $'\n'"Running program . . ."
cd $OUT_DIR
(set -x; ./$TARGET);

## Run GNUPlot subscripts
echo $'\n'"Plotting Data . . ."
(set -x; 
    gnuplot ../$P_INIT;
    gnuplot ../$P_FIN;
);

## Remove files
rm *.dat $TARGET;

## Return
cd ../
echo $'\n'"SUCCESS!"
echo "See .png files inside the 'bin' directory for generated plots"