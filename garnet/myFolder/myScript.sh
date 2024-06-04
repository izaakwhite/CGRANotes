#!/bin/bash

# Set up the correct python environment
source /aha/bin/activate

# Navigate to the CGRA example directory
cd /aha/garnet

# Compile the Hello World C++ program
g++ -o /aha/garnet/myFolder/hello_world /aha/garnet/myFolder/hello_world.cpp

# Run garnet.py with the input application option & output file
python garnet.py --width 4 --height 2 --verilog --input-app /aha/garnet/myFolder/hello_world --output-file /aha/garnet/myFolder/output.txt
