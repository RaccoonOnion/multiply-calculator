# The shell script for testing valid and invalid inputs
#!/bin/bash

# link and compile the source code to generate a.out executable file
g++ source.cpp -lmpfr -lgmp -lgmpxx

# invalid input format
echo "Inputs: a 1"
./a.out a 1
echo "Inputs: a1 1"
./a.out a1 1
echo "Inputs: 1a 1"
./a.out 1a 1
echo "Inputs: 1+1 1"
./a.out 1+1 1
echo "Inputs: 1-1 1"
./a.out 1-1 1
echo "Inputs: 1.0e1.2 1"
./a.out 1.0e1.2 1
echo "Inputs: 1.0e1e1 1"
./a.out 1.0e1e1 1
echo "Inputs: 1.1.1 1"
./a.out 1.1.1 1
echo "Inputs: 1. 1"
./a.out 1. 1
echo "Inputs: 1.e12 1"
./a.out 1.e12 1
echo "Inputs: e1 1"
./a.out e1 1

# valid input formats
echo "Inputs: 2 3"
./a.out 2 3
echo "Inputs: 3.1416 2"
./a.out 3.1416 2
echo "Inputs: 3.1415 2.0e-2"
./a.out 3.1415 2.0e-2

echo "Inputs: 1234567890 1234567890"
./a.out 1234567890 1234567890
echo "Inputs: 12345678901112131415161718192021222324252627282930 12345678901112131415161718192021222324252627282930"
./a.out 12345678901112131415161718192021222324252627282930 12345678901112131415161718192021222324252627282930
echo "Inputs: 1.0e200 1.0e200"
./a.out 1.0e200 1.0e200
echo "Inputs: 1.0e20000 1.0e20000"
./a.out 1.0e20000 1.0e20000

