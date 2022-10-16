// Macros
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <unordered_map>
// #include <list>
// #include <queue>
#include <deque>
// GMP and MPFR dependencies
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#include <gmp.h> // GMP 
#include <mpfr.h> // MPFR

// header files
#include "check.hpp"
#include "compute.hpp"

//buffer
#define BUFF_SIZE 1024

// global variable
stack<mpz_t*> nums_int;
mpz_t answer_int;
stack<mpfr_t*> nums_float;
mpfr_t answer_float;

using namespace std;

typedef struct _int_float_pointer{
    bool if_error;
    bool if_int;
    mpz_t* int_pointer;
    mpfr_t* float_pointer; 
} int_float_pointer;

class Operation_Priority
{
public:
    unordered_map<char,int> priority_map = {
            {'+',1},
            {'-',1},
            {'*',2},
            {'/',2},
            {'%',2},
            {'^',3}
    };
};

int_float_pointer calculate(const string &str)
{   
    // create instance op and element queue
    Operation_Priority op;
    deque<string> element_queue = {}; 
    bool int_mode = true;

    // split the string and get to know int mode
    int last_position = 0;
    for (int i = 0; i < str.size(); i++)
    {
        if(op.priority_map.contains(str[i]) || str[i] == '(' || str[i] == ')')
        {
            if (op.priority_map.contains(str[i-1]) || str[i-1] == '(' || str[i-1] == ')')
            {
                last_position = i+1;
                element_queue.push_back(str.substr(i,1));
                continue;
            }
            
            string tmp = str.substr(last_position, i-last_position);
            if(!detect_valid(tmp))
            {
                cerr << "Your input contains invalid operands!" << tmp << endl;
                int_float_pointer ptr = {true, false, NULL, NULL};
                return ptr;
            }
            else
            {
                if(int_mode)
                {
                    if ( str[i] == '/' || !detect_int(tmp))
                    int_mode = false;
                }
                element_queue.push_back(tmp);
                element_queue.push_back(str.substr(i,1));
                last_position = i+1;
            }   
        }
        if (i == str.size() - 1)
        {
            string tmp = str.substr(last_position, i-last_position + 1);
            if(!detect_valid(tmp))
            {
                cerr << "Your input contains invalid operands!" << tmp << endl;
                int_float_pointer ptr = {true, false, NULL, NULL};
                return ptr;
            }
            if(int_mode)
            {
                if (!detect_int(tmp))
                    int_mode = false;
            }
            element_queue.push_back(tmp);
        }
    }
    
    // calculate
    stack<char> operators;
    if (int_mode)
    {
        // add 0 in the front
        mpz_t zero;
        mpz_init(zero);
        nums_int.push(&zero);
        // total number of elements
        int n = element_queue.size();
        // assign pointers
        mpz_t * A = (mpz_t *) malloc((2 * n) * BUFF_SIZE); // buffer = 1024
        int index = 0; // index of pointers
        for (int i = 0; i < n; i++)
        {   
            string element = element_queue.at(i); // this element
            // if element is a number
            if (element.length() != 1 || isdigit(element[0]))
            {   
                const char* a_string = element.c_str();
                mpz_init_set_str((A[index]), a_string, 10);
                nums_int.push(&A[index]);
                index++;
            }
            // if element is '('
            else if (element.starts_with('(') == 1)
            {   
                operators.push('(');
                if (element_queue.at(i+1).compare("-") == 0)
                {
                    i++;
                    nums_int.push(&zero);
                    operators.push('-');
                }
            }
            // if element is ')'
            else if(element.compare(")") == 0)
            {
                while(operators.top()!='(')
                {
                    mpz_init(A[index]); // a
                    mpz_init(A[index+1]); //b
                    mpz_init(A[index+2]); // result
                    mpz_set(A[index+1], *nums_int.top()); nums_int.pop();
                    mpz_set(A[index], *nums_int.top()); nums_int.pop();
                    char oper = operators.top(); operators.pop();
                    compute_int(A[index], A[index+1], oper, A[index+2]);
                    nums_int.push(&A[index+2]);
                    index += 3;
                }
                operators.pop();
            }

            // operators
            else
            {
                // if possible calculate first!
                while(!operators.empty() && operators.top()!= '(' && op.priority_map[operators.top()] >= op.priority_map[element[0]])
                {
                    mpz_init(A[index]); // a
                    mpz_init(A[index+1]); //b
                    mpz_init(A[index+2]); // result
                    mpz_set(A[index+1], *nums_int.top()); nums_int.pop();
                    mpz_set(A[index], *nums_int.top()); nums_int.pop();
                    char oper = operators.top(); operators.pop();
                    compute_int(A[index], A[index+1], oper, A[index+2]);
                    nums_int.push(&A[index+2]);
                    index += 3;
                }
                operators.push(element[0]);
            }

        }
        // we have iterate all characters, now calculate
        while(!operators.empty() && operators.top()!='('){
            mpz_init(A[index]); //a
            mpz_init(A[index+1]); //b
            mpz_init(A[index+2]); //result

            mpz_set(A[index+1], *nums_int.top()); nums_int.pop();
            mpz_set(A[index], *nums_int.top()); nums_int.pop();
            char oper = operators.top(); operators.pop();
            compute_int(A[index], A[index+1], oper, A[index+2]);
            nums_int.push(&A[index+2]);
            index += 3;

        }
        int_float_pointer result_ptr = {false, true, nums_int.top(), NULL};
        mpz_set(answer_int, *(nums_int.top()));
        return result_ptr;
    }
    else
    {
        mpfr_prec_t prec = 1024; // 1024 bits
        mpfr_rnd_t rnd = MPFR_RNDN;
        // add 0 in the front
        mpfr_t zero;
        mpfr_init2 (zero, prec);
        nums_float.push(&zero);

        // total number of elements
        int n = element_queue.size();
        // assign pointers
        mpfr_t * A = (mpfr_t *) malloc((2 * n) * BUFF_SIZE); // buffer = 1024
        int index = 0; // index of pointers
        for (int i = 0; i < n; i++)
        {   
            string element = element_queue.at(i); // this element
            // if element is a number
            if (element.length() != 1 || isdigit(element[0]))
            {   
                const char* a_string = element.c_str();
                mpfr_init2(A[index], prec);
                mpfr_set_str((A[index]), a_string, 10, rnd);
                nums_float.push(&A[index]);
                index++;
            }
            // if element is '('
            else if (element.starts_with('(') == 1)
            {   
                operators.push('(');
                if (element_queue.at(i+1).compare("-") == 0)
                {
                    i++;
                    nums_float.push(&zero);
                    operators.push('-');
                }
            }
            // if element is ')'
            else if(element.compare(")") == 0)
            {
                while(operators.top()!='(')
                {
                    mpfr_init2(A[index], prec); // a
                    mpfr_init2(A[index+1], prec); //b
                    mpfr_init2(A[index+2], prec); // result
                    mpfr_set(A[index+1], *nums_float.top(), rnd); nums_float.pop();
                    mpfr_set(A[index], *nums_float.top(), rnd); nums_float.pop();
                    char oper = operators.top(); operators.pop();
                    compute_float(A[index], A[index+1], oper, A[index+2]);
                    nums_float.push(&A[index+2]);
                    index += 3;
                }
                operators.pop();
            }

            // operators
            else
            {
                // if possible calculate first!
                while(!operators.empty() && operators.top()!= '(' && op.priority_map[operators.top()] >= op.priority_map[element[0]])
                {
                    mpfr_init2(A[index], prec); // a
                    mpfr_init2(A[index+1], prec); //b
                    mpfr_init2(A[index+2], prec); // result
                    mpfr_set(A[index+1], *nums_float.top(), rnd); nums_float.pop();
                    mpfr_set(A[index], *nums_float.top(), rnd); nums_float.pop();
                    char oper = operators.top(); operators.pop();
                    compute_float(A[index], A[index+1], oper, A[index+2]);
                    nums_float.push(&A[index+2]);
                    index += 3;
                }
                operators.push(element[0]);
            }

        }
        // we have iterate all characters, now calculate
        while(!operators.empty() && operators.top()!='('){
            mpfr_init2(A[index], prec); // a
            mpfr_init2(A[index+1], prec); //b
            mpfr_init2(A[index+2], prec); // result
            mpfr_set(A[index+1], *nums_float.top(), rnd); nums_float.pop();
            mpfr_set(A[index], *nums_float.top(), rnd); nums_float.pop();
            char oper = operators.top(); operators.pop();
            compute_float(A[index], A[index+1], oper, A[index+2]);
            nums_float.push(&A[index+2]);
            index += 3;
        }
        int_float_pointer result_ptr = {false, false, NULL, nums_float.top()};
        mpfr_set(answer_float, *(nums_float.top()), rnd);

        mpfr_exp_t exp;
        string s = mpfr_get_str(NULL, &exp, 10, 0, answer_float, rnd);
        cout << s << endl;
        cout << exp << endl;
        return result_ptr;
    }


}

int main()
{
    // init answer
    mpz_init(answer_int);
    mpfr_init(answer_float);
    // get input
    string input;
    //getline(cin, input);
    input = "0.5*3"; //"(-3.5+2)*3"
    //calculate
    int_float_pointer ptr = calculate(input);
    //print
    // mpfr_rnd_t rnd = MPFR_RNDN;
    // mpfr_exp_t exp;
    // string s = mpfr_get_str(NULL, &exp, 10, 0, answer_float, rnd);
    cout << "answer is: " << answer_int << endl;

}
