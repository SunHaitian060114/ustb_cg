#include "fizzBuzz.h"
#include <iostream>
#include <cstring>
#include <string>
using namespace std;


string fizzBuzz(int value)
{
	if (!(value % 3) && !(value % 5))
		return "FizzBuzz";
	if (!(value % 3))
		return "Fizz";
	if (!(value % 5))
		return "Buzz";
	return to_string(value);
}


