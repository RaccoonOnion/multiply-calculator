#include <iostream>
using namespace std;

/*
Below is calculator 1.0.
Input: two ints
Output: result
*/
int main(int argc, char* argv[])
{
int x = stoi(argv[1]), y = stoi(argv[2]);

// cin >> x;
// cin >> y;

cout << x << " * " << y << " = " << x * y << endl;

return 0;
}