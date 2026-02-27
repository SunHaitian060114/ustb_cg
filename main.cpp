#include <iostream>
#include "fizzbuzz01.h"
using namespace std;
#include<gtest/gtest.h>


int main()
{
	int value;
	cin >> value;
	cout << fizzBuzz(value) << endl;
	return 0;
}

