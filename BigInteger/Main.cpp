#include <iostream>

#include "BigInteger.h"

BigInteger Fibonacci(std::uint32_t n)
{
    if (n == 0)
    {
        return BigInteger(0);
    }
    if (n == 1)
    {
        return BigInteger(1);
    }

    BigInteger prev{ 0 };
    BigInteger curr{ 1 };

    for (std::uint32_t i = 2; i <= n; ++i)
    {
        BigInteger next = prev + curr;
        prev = std::move(curr);
        curr = std::move(next);
    }

    return curr;
}

BigInteger Factorial(std::uint32_t n)
{
    if (n == 0 || n == 1)
    {
        return BigInteger(1);
    }

    BigInteger result(1);

    for (std::uint32_t i = 2; i <= n; ++i)
    {
        result *= i;
    }

    return result;
}

int main()
{
    BigInteger bigA{ "43290532525179818" };
    BigInteger bigB{ "98237498163884773" };

    std::cout << (bigA * bigB).ToString();
}