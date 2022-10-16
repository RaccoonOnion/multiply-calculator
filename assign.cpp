#include <string>
// GMP and MPFR dependencies
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#include <gmp.h> // GMP 
#include <mpfr.h> // MPFR

#include "assign.hpp"

using namespace std;

void assign(mpz_t &target, const mpz_t &source)
{
    mpz_init(target);
    mpz_set(target, source);
}
void assign(mpz_t &target, const string &source)
{
    const char* source_string = source.c_str();
    mpz_init_set_str(target, source_string, 10);
}
void assign(mpfr_t &target, const string &source, const mpfr_prec_t prec, const mpfr_rnd_t rnd)
{
    const char* source_string = source.c_str();
    mpfr_init2(target, prec);
    mpfr_set_str(target, source_string, 10, rnd);
}
void assign(mpfr_t &target, const mpfr_t &source, const mpfr_prec_t prec, const mpfr_rnd_t rnd)
{
    mpfr_init2(target, prec); // a
    mpfr_set(target, source, rnd);
}

