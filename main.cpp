#include <iostream>
#include "fizzbuzz.h"
using namespace std;
//#include"gtest/gtest.h"

void test(int input, string output, int aim)
{
	//The input corresponds to the intput value in TodoList
	//The output corresponds to the expected output
	//The aim corresponds to the serial number in TodoList
	string result = fizzBuzz(input);
	if (result == output)
	{
		cout << "Test " << aim << " passed. Expected: " << output << ", got: " << result << endl;
	}
	else
	{
		cout << "Test " << aim << " failed. Expected: " << output << ", got: " << result << endl;
	}

}

int main()
{
    int value;
    //cin >> value;
    //cout << fizzBuzz(value) << endl;
	test(3, "Fizz", 4);
    return 0;
}
