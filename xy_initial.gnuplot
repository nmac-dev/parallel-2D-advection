### Requirements
## Input Data: "initial.dat" which contains 3 columns: [x,y,u]
## Output PNG: "initial.png" sent to current working directory 
## Warning: Script must run within the directory containing the intput data

# Set output as PNG
set terminal png enhanced 
# Set output filename
set output "initial.png"

# Set axes ranges
set xrange [0:30.0]
set yrange [0:30.0]

# Set axes labels
set xlabel "Horizontal (x)"
set ylabel "Vertical (y)"

# Set aspect ratio (1)
set size square

# Set colour scale range
set cbrange [0:1]

# Plot Data 
plot "initial.dat" with image 