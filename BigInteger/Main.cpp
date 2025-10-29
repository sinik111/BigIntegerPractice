#include <iostream>

#include "BigInteger.h"

int main()
{
	BigInteger a{ "58249375892037584735809723758432758920543857489759483758947358978943758934" };
	BigInteger b{ "48374809137580974329085728394570982375584390570983475983475983479857349778" };

	std::cout << "a = " << a << std::endl;
	std::cout << "b = " << b << std::endl;
	std::cout << "a + b = " << a + b << std::endl;
	std::cout << "a - b = " << a - b << std::endl;
	std::cout << "a * b = " << a * b << std::endl;
	std::cout << "a / b = " << a / b << std::endl;
	std::cout << "a % b = " << a % b << std::endl;
}