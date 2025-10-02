#include "BigInteger.h"

BigInteger::BigInteger()
	: m_digits{ 0 }, m_isNegative{ false }
{

}

BigInteger::BigInteger(std::int32_t number)
	: m_isNegative{ number < 0 }
{
	if (number == 0)
	{
		m_digits.push_back(0);

		return;
	}

	std::int64_t absNumber = m_isNegative ? -static_cast<std::int64_t>(number) : static_cast<std::int64_t>(number);

	while (absNumber > 0)
	{
		m_digits.push_back(static_cast<std::int32_t>(absNumber % BigInteger::BASE));

		absNumber /= BigInteger::BASE;
	}
}

BigInteger::BigInteger(std::uint32_t number)
	: m_isNegative{ false }, m_digits{ number }
{
	if (number == 0)
	{
		m_digits.push_back(0);

		return;
	}

	std::uint64_t tempNumber = number;

	while (tempNumber > 0)
	{
		m_digits.push_back(static_cast<std::int32_t>(tempNumber % BigInteger::BASE));

		tempNumber /= BigInteger::BASE;
	}
}

BigInteger::BigInteger(std::int64_t number)
	: m_isNegative{ number < 0 }
{
	if (number == 0)
	{
		m_digits.push_back(0);

		return;
	}

	std::uint64_t absNumber = m_isNegative ? 0ULL - static_cast<std::uint64_t>(number) : static_cast<std::uint64_t>(number);

	while (absNumber > 0)
	{
		m_digits.push_back(static_cast<std::uint32_t>(absNumber % BigInteger::BASE));

		absNumber /= BigInteger::BASE;
	}
}

BigInteger::BigInteger(std::uint64_t number)
	: m_isNegative{ false }
{
	if (number == 0)
	{
		m_digits.push_back(0);

		return;
	}

	while (number > 0)
	{
		m_digits.push_back(static_cast<std::uint32_t>(number % BigInteger::BASE));

		number /= BigInteger::BASE;
	}
}

BigInteger::BigInteger(const std::string& number)
{
	if (number.empty())
	{
		throw;
	}

	m_isNegative = number[0] == '-';

	long long limit = m_isNegative ? 1 : 0;
	long long position = number.size() - 1;

	while (position >= limit)
	{
		std::uint64_t tempNumber = 0;
		size_t counter = 0;

		while (counter < 9 && position >= limit)
		{
			tempNumber += (number[position] - '0') * POW_TEN[counter];

			++counter;
			--position;
		}

		m_digits.push_back(static_cast<std::uint32_t>(tempNumber % BigInteger::BASE));
	}
}

BigInteger::BigInteger(const BigInteger& other)
	: m_digits{ other.m_digits }, m_isNegative{ other.m_isNegative }
{
}

BigInteger& BigInteger::operator=(const BigInteger& other)
{
	m_digits = other.m_digits;
	m_isNegative = other.m_isNegative;

	return *this;
}

BigInteger::BigInteger(BigInteger&& other) noexcept
	: m_digits{ std::move(other.m_digits) }, m_isNegative{ other.m_isNegative }
{

}

BigInteger& BigInteger::operator=(BigInteger&& other) noexcept
{
	m_digits = std::move(other.m_digits);
	m_isNegative = other.m_isNegative;

	return *this;
}

BigInteger::BigInteger(std::vector<std::uint32_t>&& digits, bool isNegative)
	: m_digits{ std::move(digits) }, m_isNegative{ isNegative }
{
	
}

BigInteger BigInteger::operator+(const BigInteger& other)
{
	std::vector<std::uint32_t> tempDigits;

	size_t thisSize = m_digits.size();
	size_t thatSize = other.m_digits.size();

	size_t position = 0;
	std::uint64_t tempNumber = 0;

	while (position < thisSize && position < thatSize)
	{
		std::int64_t thisNum = static_cast<std::int64_t>(m_digits[position]);
		std::int64_t thatNum = static_cast<std::int64_t>(other.m_digits[position]);

		if ((m_isNegative && other.m_isNegative) || (!m_isNegative && !other.m_isNegative))
		{
			tempNumber += (thisNum + thatNum);
		}
		else
		{

		}
		

		tempDigits.push_back(static_cast<std::uint32_t>(tempNumber % BASE));

		tempNumber /= BASE;
	}

	while (thisSize > position)
	{

	}

	while (thatSize > position)
	{

	}

	return BigInteger();
}

BigInteger BigInteger::operator-(const BigInteger& other)
{
	return BigInteger();
}


bool BigInteger::operator<(const BigInteger& other) const
{
	if (m_isNegative != other.m_isNegative)
	{
		return m_isNegative;
	}

	size_t thisSize = m_digits.size();
	size_t thatSize = other.m_digits.size();

	if (thisSize != thatSize)
	{
		return m_isNegative ? (thisSize > thatSize) : (thisSize < thatSize);
	}
	
	for (long long i = static_cast<long long>(thisSize) - 1; i >= 0; --i)
	{
		if (m_digits[i] != other.m_digits[i])
		{
			return m_isNegative ? m_digits[i] > other.m_digits[i] : m_digits[i] < other.m_digits[i];
		}
	}

	return false;
}

bool BigInteger::operator>(const BigInteger& other) const
{
	return !(*this == other) && !(*this < other);
}

bool BigInteger::operator<=(const BigInteger& other) const
{
	if (*this == other)
	{
		return true;
	}

	return *this < other;
}

bool BigInteger::operator>=(const BigInteger& other) const
{
	if (*this == other)
	{
		return true;
	}

	return !(*this < other);
}

bool BigInteger::operator==(const BigInteger& other) const
{
	if (m_isNegative != other.m_isNegative)
	{
		return false;
	}

	if (m_digits.size() != other.m_digits.size())
	{
		return false;
	}

	for (size_t i = 0; i < m_digits.size(); ++i)
	{
		if (m_digits[i] != other.m_digits[i])
		{
			return false;
		}
	}

	return true;
}

bool BigInteger::operator!=(const BigInteger& other) const
{
	return !(*this == other);
}