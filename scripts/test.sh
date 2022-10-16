# The shell script for testing the calculator program
#!/bin/bash

# link and compile the source code to generate a.out executable file
g++ source.cpp check.cpp compute.cpp assign.cpp -lmpfr -lgmp -lgmpxx --std=c++20

#####################################################################################
# Integer operations 
echo "2+3"   
./a.out <<< "2+3"
echo "2*3"
./a.out <<< "2*3"
echo "2-3"
./a.out <<< "2-3"
echo "2%3"
./a.out <<< "2%3"
echo "2^3"
./a.out <<< "2^3"
echo "-2+3"
./a.out <<< "-2+3"
echo "-2*3"
./a.out <<< "-2*3"
echo "-2-3"
./a.out <<< "-2-3"
echo "-2%3"
./a.out <<< "-2%3"
echo "-2^3"
./a.out <<< "-2^3"
# Some operations that have to be floating point operation
echo "2/3"
./a.out <<< "2/3"

# Parentheses and priority
echo "5+2*3"
./a.out <<< "5+2*3"
echo "(5+2)*3"
./a.out <<< "(5+2)*3"
#####################################################################################

#####################################################################################
# Floating point operations
echo "0.5+2"
./a.out <<< "0.5+2"
echo "0.5-2"
./a.out <<< "0.5-2"
echo "0.5*2"
./a.out <<< "0.5*2"
echo "0.5/2"
./a.out <<< "0.5/2"
echo "0.5^2"
./a.out <<< "0.5^2"
echo "0.5%2"          # modulo operation is not well defined on floats, get errors
./a.out <<< "0.5%2"
echo "-0.5+2*3"
./a.out <<< "-0.5+2*3"

# Parentheses and priority
echo "0.5+2*3"
./a.out <<< "0.5+2*3"
echo "(0.5+2)*3"
./a.out <<< "(0.5+2)*3"
#####################################################################################

#####################################################################################
# Large integers and large floats

echo "2000000000000000000000000000000+3"   
./a.out <<< "2000000000000000000000000000000+3"
echo "2000000000000000000000000000000*3"
./a.out <<< "2000000000000000000000000000000*3"
echo "2000000000000000000000000000000-3"
./a.out <<< "2000000000000000000000000000000-3"
echo "2000000000000000000000000000000%3"
./a.out <<< "2000000000000000000000000000000%3"
echo "2000000000000000000000000000000^3"
./a.out <<< "2000000000000000000000000000000^3"

echo "2.0e200+3.0e200"   
./a.out <<< "2.0e200+3.0e200"
echo "2.0e200*3.0e200"
./a.out <<< "2.0e200*3.0e200"
echo "2.0e200-3.0e200"
./a.out <<< "2.0e200-3.0e200"
echo "2.0e200%3"
./a.out <<< "2.0e200%3"
echo "2.0e200^3"
./a.out <<< "2.0e200^3"

#####################################################################################

#####################################################################################
 

#####################################################################################












