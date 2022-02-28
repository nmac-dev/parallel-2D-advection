### Requirements
## Input Data: "v_averaged.dat" which contains 3 columns: [x,y,u]
## Output PNG: "v_averaged.png" sent to current working directory 
## Warning: Script must run within the directory containing the intput data

# Set output as PNG
set terminal png enhanced 
# Set output filename
set output "v_averaged.png"

# Do not set ranges, instead gnuplot will auto set the values

# Set axes labels
set xlabel "Horizontal (x)"
set ylabel "Vertically Averaged Distribution of u(x,y)"

# Plot Data 
plot "v_averaged.dat" with lines 