// Macros
#include <iostream>
#include <string>

// GMP and MPFR dependencies
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#include <gmp.h> // GMP 
#include <mpfr.h> // MPFR

// header files
#include "check.hpp"
#include "compute.hpp"

using namespace std;

void compute_int(mpz_t a, mpz_t b, char op, mpz_t result)
{
    try {
        if (op == '+' || op == '-' || op == '*' || op == '%' || op == '^')
        {
            switch (op) 
            {
                // functions to calculate the result
                case '+':   mpz_add(result, a, b); break;
                case '-':   mpz_sub(result, a, b); break;
                case '*':   mpz_mul(result, a, b); break;
                case '%':   mpz_mod(result, a, b); break;
                case '^':   mpz_pow_ui(result, a, mpz_get_ui(b) ); break;
            }
            return;
        }
    }
    catch (const std::exception & ex )
    {
        cerr << ex.what() << endl; // print the error message
    }
}

void compute_float(mpfr_t a, mpfr_t b, char op, mpfr_t result)
{
    try {
            mpfr_rnd_t rnd = MPFR_RNDN; // the rounding mode, as defined by IEEE 754-1985 standard, 3 more rounding mode are available

            switch (op) {
                case '+':   mpfr_add(result, a, b, rnd); break;
                case '-':   mpfr_sub(result, a, b, rnd); break;
                case '*':   mpfr_mul(result, a, b, rnd); break;
                case '/':   mpfr_div(result, a, b, rnd); break;
                case '^':   mpfr_pow(result, a, b, rnd); break;
                default :   throw invalid_argument( "Operations are not supported on floating point numbers!" );
            }
        }
    catch (const std::exception & ex )
    {
        cerr << ex.what() << endl; // print the error message
    }

}
