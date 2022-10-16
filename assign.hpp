#pragma once

#include <string>
// GMP and MPFR dependencies
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#include <gmp.h> // GMP 
#include <mpfr.h> // MPFR

using namespace std;

void assign(mpz_t &target, const mpz_t &source);
void assign(mpz_t &target, const string &source);
void assign(mpfr_t &target, const string &source, const mpfr_prec_t prec, const mpfr_rnd_t rnd);
void assign(mpfr_t &target, const mpfr_t &source, const mpfr_prec_t prec, const mpfr_rnd_t rnd);
