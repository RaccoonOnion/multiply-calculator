// Macros
#include <iostream>
#include <string>
// GMP and MPFR dependencies
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#include <gmp.h> // GMP 
#include <mpfr.h> // MPFR

using namespace std;


// Part One: The helper functions.

/* 
Function to detect if the input is valid or not.
This part of code is based on the work of rahulkumawat2107 from https://www.geeksforgeeks.org/check-given-string-valid-number-integer-floating-point/
Some errors in the original are fixed and some extensions are added to make the checking process more well-rounded.
*/
bool detect_valid(const string &str) // By using const we avoid the possible distortion of original stringd
{
	long i = 0, j = str.length() - 1;

	// if string is of length 1 and the only
	// character is not a digit
	if (i == j && !(str[i] >= '0' && str[i] <= '9'))
		return 0;

	// To check if a '.' or 'e' is found in given
	// string. We use this flag to make sure that
	// either of them appear only once.
	bool flag_e = false;
    bool flag_dot = false;

	for (; i <= j; i++) {
		// If any of the char does not belong to
		// {digit, +, -, ., e}
		if (str[i] != 'e' && str[i] != '.'
			&& str[i] != '+' && str[i] != '-'
			&& !(str[i] >= '0' && str[i] <= '9'))
			return 0;

		if (str[i] == '.') {
			// checks if the char 'e' or '.' has already
			// occurred before this '.' If yes, return 0.
			if (flag_e == true || flag_dot == true)
				return 0;

			// If '.' is the last character.
			if (i + 1 > str.length())
				return 0;

			// if '.' is not followed by a digit.
			if (!(str[i + 1] >= '0' && str[i + 1] <= '9'))
				return 0;
            // set flag_dot = 1 when . is encountered.            
            flag_dot = true;
		}

		else if (str[i] == 'e') {
            // if e has appeared before
            if (flag_e)
                return 0;

			// if there is no digit before 'e'.
			if (!(str[i - 1] >= '0' && str[i - 1] <= '9'))
				return 0;

			// If 'e' is the last Character
			if (i + 1 == str.length())
				return 0;

			// if e is not followed either by
			// '+', '-' or a digit
			if (str[i + 1] != '+' && str[i + 1] != '-'
				&& (str[i + 1] >= '0' && str[i] <= '9'))
				return 0;

            // set flag_e = 1 when e is encountered.
            flag_e = true;
		}
        else if (str[i] == '+' || str[i] == '-')
        {
            if (str[i-1] != 'e')
                return 0;
        }
	}

	// If the string skips all above cases, then it is numeric
	return 1;
}

/* 
Function to detect if an input string is integer or floating-point number.
Assume that the input passes the validity test.
*/
bool detect_int(const string &s)
{
    for (long i = 0; i < s.length(); i++)
    {
        if (isdigit(s[i]) == false) // if any one digit is not a number between 0 and 9 then this number is not an integer
            return false;
    }
    return true;
}

int main(int argc, char* argv[])
{
    bool int_mode = false; // boolean variable indicating if the integer method should be used or not

    // Checking for the number of inputs
    if (argc != 3)
    {
        std::cout << "The number of your inputs is not correct!\nPlease input two numbers." << endl;
        return -1;
    }

    // If the number of inputs is 2, store the command line arguments for further use
    string input1 = argv[1];
    string input2 = argv[2];

    // Using detect_valid function to check the input validity
    if ( !detect_valid(input1) || !detect_valid(input2))
    {
        cerr << "Your inputs cannot be interpret as numbers!" << endl;
        return -1;
    }

    // Using detect_int function to check the input type
    if (detect_int(input1) && detect_int(input2))
        int_mode = true;

    // Depending on the value of int_mode, we use two different set of variable types to compute the result

    const char* a_string = input1.c_str(); // converting string object to const char* for further usage
    const char* b_string = input2.c_str();
    try {
        if (int_mode)
        {
            mpz_t a, b, result; // Declare the integer variable type, mpz_t
            mpz_init_set_str(a, a_string, 10); // initialize and assign value. a_string -> a. Base 10
            mpz_init_set_str(b, b_string, 10);
            mpz_mul(result, a, b); // function to multiply a and b and assign the value in result
            std::cout << a << " * " << b << " = " << result << endl; // print the result in format
            mpz_clear (a);
            mpz_clear (b);
            mpz_clear (result); // free the memory
        }

        else
        {
            mpfr_t a, b, result; // declare floating-point variable type: mpfr_t
            mpfr_prec_t prec = 1024; // declare the precision (in bits) used for mpfr_t, 
            // may be set to any value not exceeding current available memory
            std::cout << "The current precision is 1024 bits, you may modify it as long as the enough memory can be assigned." << endl;

            mpfr_init2 (a, prec);
            mpfr_init2 (b, prec);
            mpfr_init2 (result, prec); // initialize variable with precision

            mpfr_rnd_t rnd = MPFR_RNDN; // the rounding mode, as defined by IEEE 754-1985 standard, 3 more rounding mode are available

            mpfr_set_str(a, a_string, 10, rnd);
            mpfr_set_str(b, b_string, 10, rnd);// assign the value to a and b using const char* a_string and b_string, base 10

            mpfr_mul(result, a, b, rnd);// multiply a and b and store the value in result

            std::cout << input1 << " * " << input2 << " = ";

            // standard output of result in base 10, third parameter can control how many significant digits to print
            mpfr_out_str (stdout, 10, 10, result, rnd);                                 
            std::cout << endl;

            mpfr_clear (a);
            mpfr_clear (b);
            mpfr_clear (result);
            mpfr_free_cache (); // free the memory
        }
    }
    catch (const std::exception & ex )
    {
        cerr << ex.what() << endl; // print the error message
    }

return 0;

}
