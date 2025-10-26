#include <iostream>

#include "BigInteger.h"

int main()
{
	BigInteger num1{ "3548915719847123897423891047012" };
	BigInteger num2{ "-3548915719847123897423891047012" };

	std::cout << num1.ToString() << std::endl;
	std::cout << num2.ToString() << std::endl;
}