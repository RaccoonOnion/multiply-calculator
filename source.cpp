// Macros
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <unordered_map>
#include <regex>
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
#include "assign.hpp"

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

//constants
#define BUFF_SIZE 1024
const mpfr_prec_t prec = 1024 * 8;
const mpfr_rnd_t rnd = MPFR_RNDN;
const Operation_Priority op;

// global variable
stack<mpz_t*> nums_int;
stack<mpfr_t*> nums_float;

using namespace std;

typedef struct _result_tuple{
    bool if_error;
    bool if_int;
} result_tuple;

// class Operation_Priority
// {
// public:
//     unordered_map<char,int> priority_map = {
//             {'+',1},
//             {'-',1},
//             {'*',2},
//             {'/',2},
//             {'%',2},
//             {'^',3}
//     };
// };

result_tuple split(const string &str, const Operation_Priority &op, deque<string> &element_queue)
{
    // split the string and get to know int mode
    int last_position = 0;
    bool int_mode = true;
    for (int i = 0; i < str.size(); i++)
    {
        if (i == 0 && str[i] == '-')
        {
            continue;
        }
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
                result_tuple ptr = {true, false};
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
                result_tuple ptr = {true, false};
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
    result_tuple ptr = {false, int_mode};
    return ptr;
} 

result_tuple calculate(const string &str, mpz_t answer_int, mpfr_t answer_float)
{   
    // create instance op and element queue
    Operation_Priority op;
    deque<string> element_queue = {}; 
    bool int_mode = true;
    bool error_mode = false;

    result_tuple ptr = split(str, op, element_queue);
    int_mode = ptr.if_int;
    error_mode = ptr.if_error;
    
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
                assign(A[index], element);
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
                    assign(A[index+1], *nums_int.top()); nums_int.pop();
                    assign(A[index], *nums_int.top()); nums_int.pop();  
                    mpz_init(A[index+2]);                  
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
                    assign(A[index+1], *nums_int.top()); nums_int.pop();
                    assign(A[index], *nums_int.top()); nums_int.pop(); 
                    mpz_init(A[index+2]);  
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
            assign(A[index+1], *nums_int.top()); nums_int.pop();
            assign(A[index], *nums_int.top()); nums_int.pop();  
            mpz_init(A[index+2]);  
            char oper = operators.top(); operators.pop();
            compute_int(A[index], A[index+1], oper, A[index+2]);
            nums_int.push(&A[index+2]);
            index += 3;

        }
        result_tuple result_ptr = {false, true};
        mpz_set(answer_int, *(nums_int.top()));
        // memory management
        for (int k = 0; k < index; k ++)
        {
            mpz_clear (A[k]);
        }
        delete[](A);

        return result_ptr;
    }
    else
    {
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
                assign(A[index], element, prec, rnd);
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
                    // mpfr_init2(A[index], prec); // a
                    // mpfr_init2(A[index+1], prec); //b
                    // mpfr_init2(A[index+2], prec); // result
                    // mpfr_set(A[index+1], *nums_float.top(), rnd); nums_float.pop();
                    // mpfr_set(A[index], *nums_float.top(), rnd); nums_float.pop();
                    assign(A[index+1], *nums_float.top(), prec, rnd); nums_float.pop();
                    assign(A[index], *nums_float.top(), prec, rnd); nums_float.pop(); 
                    mpfr_init2(A[index+2], prec); // result                
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
                    assign(A[index+1], *nums_float.top(), prec, rnd); nums_float.pop();
                    assign(A[index], *nums_float.top(), prec, rnd); nums_float.pop(); 
                    mpfr_init2(A[index+2], prec); // result  
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
            assign(A[index+1], *nums_float.top(), prec, rnd); nums_float.pop();
            assign(A[index], *nums_float.top(), prec, rnd); nums_float.pop(); 
            mpfr_init2(A[index+2], prec); // result  
            char oper = operators.top(); operators.pop();
            compute_float(A[index], A[index+1], oper, A[index+2]);
            nums_float.push(&A[index+2]);
            index += 3;
        }
        result_tuple result_ptr = {false, false};
        mpfr_set(answer_float, *(nums_float.top()), rnd);
        mpfr_exp_t exp;
        string s = mpfr_get_str(NULL, &exp, 10, 0, answer_float, rnd);
        if (s.compare("@NaN@") == 0)
        {
            result_ptr.if_error = true;
        }
        // memory management
        for (int k = 0; k < index; k++)
        {
            mpfr_clear (A[k]);
        }
        mpfr_free_cache ();
        delete[](A);
        return result_ptr;
    }
}


int main()
{
    stack<string> var;
    stack<string> exp;
    //substitution.insert("hhh", "hh");
    string input;
    getline(cin, input);
    while (input.find('=') != std::string::npos)
    {
        for(int i =0; i < input.length(); i++)
        {
            if(input[i] == '=')
            {
                var.push(input.substr(0, i));
                exp.push(input.substr(i+1, input.length() - i));
            }
        }
        getline(cin, input);
    }

    while(!var.empty()&&!exp.empty())
    {
        if(var.top().empty())
        {    
            var.pop();
            exp.pop();
            continue;
        }
        size_t start_pos = 0;
        while((start_pos = input.find(var.top(), start_pos)) != string::npos) {
            input.replace(start_pos, var.top().length(), exp.top());
            start_pos += exp.top().length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
        }
        var.pop();
        exp.pop();
    }

    mpz_t answer_int;
    mpfr_t answer_float;
    mpz_init(answer_int);
    mpfr_init2(answer_float, prec);
    result_tuple ptr = calculate(input, answer_int, answer_float);
    if (!ptr.if_error && ptr.if_int)
    {
        cout << "answer is: " << answer_int << endl;
    }
    else if(!ptr.if_error && !ptr.if_int)
    {
        cout << "floating point calculation, answer is: ";
        mpfr_out_str (stdout, 10, 10, answer_float, rnd);
        cout << endl;
    }

}
