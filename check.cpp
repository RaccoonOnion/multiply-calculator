#include "check.hpp"

/* 
Function to detect if an input string is integer or floating-point number.
Assume that the input passes the validity test.
*/
bool detect_int(const string &str)
{
    bool digit = false;
    for (long i = 0; i < str.length(); i++)
    {
        if (!digit && str[i] == ' ')
        {
            continue;
        }
        if (isdigit(str[i]) == false) // if any one digit is not a number between 0 and 9 then this number is not an integer
        {
            if (!digit) digit = true;
            return false;
        }
            
    }
    return true;
}

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
        // if the character is a space
        if (str[i] == ' ') continue;     

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

