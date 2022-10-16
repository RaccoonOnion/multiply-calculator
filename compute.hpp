#pragma once
#include <string>

// GMP and MPFR dependencies
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#include <gmp.h> // GMP 
#include <mpfr.h> // MPFR

using namespace std;

void compute_int(mpz_t a, mpz_t b, char op, mpz_t result);

void compute_float(mpfr_t a, mpfr_t b, char op, mpfr_t result);