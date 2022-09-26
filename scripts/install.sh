# Shell scipt for installing GMP and MPFR
# Before running this script, you need to download the GMP package from https://gmplib.org/#DOWNLOAD
# and MPFR package from https://mpfr.loria.fr/mpfr-current/#download.
# And unzip them to the same folder there this shell script and your C/C++ source code are

# GMP Installation
cd gmp-*
./configure --enable-cxx
make
make check
sudo make install  # need to input password to grant sudo access
cd ..

# MPFR Installation
cd mpfr-*
./configure
make
make check
sudo make install # need to input password to grant sudo access
cd ..
