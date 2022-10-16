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
    // bool int_mode = false;

    // // Using detect_int function to check the input type
    // if (detect_int(input1) && detect_int(input2))
    //     int_mode = true;
    // if (op == '%' && !int_mode)
    // {
    //     cerr << "Modulo Expression cannot be applied to non-integers!!" << endl; // error!!
    // }


    try {
        if (op == '+' || op == '-' || op == '*' || op == '%')
        {
            switch (op) 
            {
                // functions to calculate the result
                case '+':   mpz_add(result, a, b); break;
                case '-':   mpz_sub(result, a, b); break;
                case '*':   mpz_mul(result, a, b); break;
                case '%':   mpz_mod(result, a, b); break;
            }
            // std::cout << a << " * " << b << " = " << result << endl; // print the result in format

            return;
            // mpz_clear (a);
            // mpz_clear (b);
            // mpz_clear (result); // free the memory
        }
    }
    catch (const std::exception & ex )
    {
        cerr << ex.what() << endl; // print the error message
    }
}

void compute_float(mpfr_t a, mpfr_t b, char op, mpfr_t result)
{
    // const char* a_string = input1.c_str(); // converting string object to const char* for further usage
    // const char* b_string = input2.c_str();

    try {
            // mpfr_t a, b, result; // declare floating-point variable type: mpfr_t
            // mpfr_prec_t prec = 1024; // declare the precision (in bits) used for mpfr_t, 
            // // may be set to any value not exceeding current available memory
            // std::cout << "The current precision is 1024 bits, you may modify it as long as the enough memory can be assigned." << endl;

            // mpfr_init2 (a, prec);
            // mpfr_init2 (b, prec);
            // mpfr_init2 (result, prec); // initialize variable with precision

            mpfr_rnd_t rnd = MPFR_RNDN; // the rounding mode, as defined by IEEE 754-1985 standard, 3 more rounding mode are available

            // mpfr_set_str(a, a_string, 10, rnd);
            // mpfr_set_str(b, b_string, 10, rnd);// assign the value to a and b using const char* a_string and b_string, base 10

            switch (op) {
                case '+':   mpfr_add(result, a, b, rnd); break;
                case '-':   mpfr_sub(result, a, b, rnd); break;
                case '*':   mpfr_mul(result, a, b, rnd); break;
                case '/':   mpfr_div(result, a, b, rnd); break;
                case '^':   mpfr_pow(result, a, b, rnd); break;
            }

            // mpfr_clear (a);
            // mpfr_clear (b);
            // mpfr_clear (result);
            // mpfr_free_cache (); // free the memory
        }
    catch (const std::exception & ex )
    {
        cerr << ex.what() << endl; // print the error message
    }

}
