#include "BigInteger.h"

#include <cassert>
#include <sstream>

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
		m_digits.push_back(static_cast<std::uint32_t>(absNumber % BigInteger::BASE));

		absNumber /= BigInteger::BASE;
	}
}

BigInteger::BigInteger(std::uint32_t number)
	: m_isNegative{ false }
{
	if (number == 0)
	{
		m_digits.push_back(0);

		return;
	}

	std::uint64_t tempNumber = number;

	while (tempNumber > 0)
	{
		m_digits.push_back(static_cast<std::uint32_t>(tempNumber % BigInteger::BASE));

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
	if (number.empty() || !IsValid(number))
	{
		m_digits.push_back(0);
		m_isNegative = false;

		return;
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

		m_digits.push_back(static_cast<std::uint32_t>(tempNumber));
	}

	Normalize();
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

BigInteger::BigInteger(Digits&& digits, bool isNegative)
	: m_digits{ std::move(digits) }, m_isNegative{ isNegative }
{
	
}

BigInteger::BigInteger(const Digits& digits, bool isNegative)
	: m_digits{ digits }, m_isNegative{ isNegative }
{

}

BigInteger BigInteger::operator+(const BigInteger& other) const
{
	Digits tempDigits;
	bool negative = false;

	if (m_isNegative == other.m_isNegative)
	{
		negative = m_isNegative;

		Add(m_digits, other.m_digits, tempDigits);
	}
	else
	{
		if (CompareMagnitude(m_digits, other.m_digits) > 0)
		{
			negative = m_isNegative;

			Subtract(m_digits, other.m_digits, tempDigits);
		}
		else
		{
			negative = !m_isNegative;

			Subtract(other.m_digits, m_digits, tempDigits);
		}
	}

	BigInteger result = BigInteger(std::move(tempDigits), negative);
	result.Normalize();

	return result;
}

BigInteger BigInteger::operator+(std::int32_t other) const
{
	return *this + BigInteger(other);
}

BigInteger BigInteger::operator+(std::uint32_t other) const
{
	return *this + BigInteger(other);
}

BigInteger BigInteger::operator+(std::int64_t other) const
{
	return *this + BigInteger(other);
}

BigInteger BigInteger::operator+(std::uint64_t other) const
{
	return *this + BigInteger(other);
}

BigInteger BigInteger::operator-(const BigInteger& other) const
{
	Digits tempDigits;
	bool negative = false;

	if (m_isNegative == other.m_isNegative)
	{
		if (CompareMagnitude(m_digits, other.m_digits) > 0)
		{
			negative = m_isNegative;

			Subtract(m_digits, other.m_digits, tempDigits);
		}
		else
		{
			negative = !m_isNegative;

			Subtract(other.m_digits, m_digits, tempDigits);
		}
	}
	else
	{
		negative = m_isNegative;

		Add(m_digits, other.m_digits, tempDigits);
	}

	BigInteger result = BigInteger(std::move(tempDigits), negative);
	result.Normalize();

	return result;
}

BigInteger BigInteger::operator-(std::int32_t other) const
{
	return *this - BigInteger(other);
}

BigInteger BigInteger::operator-(std::uint32_t other) const
{
	return *this - BigInteger(other);
}

BigInteger BigInteger::operator-(std::int64_t other) const
{
	return *this - BigInteger(other);
}

BigInteger BigInteger::operator-(std::uint64_t other) const
{
	return *this - BigInteger(other);
}

BigInteger BigInteger::operator*(const BigInteger& other) const
{
	Digits tempDigits;

	Multiply(m_digits, other.m_digits, tempDigits);

	BigInteger result = BigInteger(std::move(tempDigits), m_isNegative != other.m_isNegative);
	result.Normalize();

	return result;
}

BigInteger BigInteger::operator*(std::int32_t other) const
{
	return *this * BigInteger(other);
}

BigInteger BigInteger::operator*(std::uint32_t other) const
{
	return *this * BigInteger(other);
}

BigInteger BigInteger::operator*(std::int64_t other) const
{
	return *this * BigInteger(other);
}

BigInteger BigInteger::operator*(std::uint64_t other) const
{
	return *this * BigInteger(other);
}

BigInteger& BigInteger::operator+=(const BigInteger& other)
{
	*this = *this + other;
	return *this;
}

BigInteger& BigInteger::operator+=(std::int32_t other)
{
	*this = *this + other;
	return *this;
}

BigInteger& BigInteger::operator+=(std::uint32_t other)
{
	*this = *this + other;
	return *this;
}

BigInteger& BigInteger::operator+=(std::int64_t other)
{
	*this = *this + other;
	return *this;
}

BigInteger& BigInteger::operator+=(std::uint64_t other)
{
	*this = *this + other;
	return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& other)
{
	*this = *this - other;
	return *this;
}

BigInteger& BigInteger::operator-=(std::int32_t other)
{
	*this = *this - other;
	return *this;
}

BigInteger& BigInteger::operator-=(std::uint32_t other)
{
	*this = *this - other;
	return *this;
}

BigInteger& BigInteger::operator-=(std::int64_t other)
{
	*this = *this - other;
	return *this;
}

BigInteger& BigInteger::operator-=(std::uint64_t other)
{
	*this = *this - other;
	return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& other)
{
	*this = *this * other;
	return *this;
}

BigInteger& BigInteger::operator*=(std::int32_t other)
{
	*this = *this * other;
	return *this;
}

BigInteger& BigInteger::operator*=(std::uint32_t other)
{
	*this = *this * other;
	return *this;
}

BigInteger& BigInteger::operator*=(std::int64_t other)
{
	*this = *this * other;
	return *this;
}

BigInteger& BigInteger::operator*=(std::uint64_t other)
{
	*this = *this * other;
	return *this;
}

BigInteger BigInteger::operator-() const
{
	BigInteger i{ m_digits, !m_isNegative };

	i.Normalize();

	return i;
}

bool BigInteger::operator<(const BigInteger& other) const
{
	if (m_isNegative != other.m_isNegative)
	{
		return m_isNegative;
	}

	const size_t selfSize = m_digits.size();
	const size_t otherSize = other.m_digits.size();

	if (selfSize != otherSize)
	{
		return m_isNegative ? (selfSize > otherSize) : (selfSize < otherSize);
	}
	
	for (long long i = static_cast<long long>(selfSize) - 1; i >= 0; --i)
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
	return (*this == other) || (*this < other);
}

bool BigInteger::operator>=(const BigInteger& other) const
{
	return (*this == other) || !(*this < other);
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

bool BigInteger::IsValid(const std::string& number)
{
	if ((number[0] < '0' || number[0] > '9') && number[0] != '-')
	{
		assert(false);

		return false;
	}

	const size_t length = number.size();
	size_t position = 0;

	if (number[position] == '-')
	{
		++position;
	}

	if (position == length)
	{
		assert(false);

		return false;
	}

	while (position < length)
	{
		if (number[position] < '0' || number[position] > '9')
		{
			assert(false);

			return false;
		}

		++position;
	}

	return true;
}

void BigInteger::Normalize()
{
	for (int i = static_cast<int>(m_digits.size()) - 1; i >= 0; --i)
	{
		if (m_digits[i] == 0)
		{
			if (i == 0)
			{
				m_isNegative = false;
			}
			else
			{
				m_digits.pop_back();
			}
		}
		else
		{
			break;
		}
	}
}

std::string BigInteger::ToString() const
{
	std::ostringstream oss;

	if (m_isNegative)
	{
		oss << '-';
	}

	if (m_digits.size() == 1 && m_digits[0] == 0)
	{
		oss << '0';
	}
	else
	{
		int size = static_cast<int>(m_digits.size());
		for (int i = size - 1; i >= 0; --i)
		{
			if (i == size - 1)
			{
				oss << m_digits[i];
			}
			else
			{
				oss << ZERO_PAD_STRINGS[GetDigitCount(m_digits[i]) - 1];
				oss << m_digits[i];
			}
		}
	}

	return oss.str();
}

BigInteger BigInteger::Abs() const
{
	return BigInteger(m_digits, false);
}

void BigInteger::Add(const Digits& a, const Digits& b, Digits& out)
{
	const size_t aSize = a.size();
	const size_t bSize = b.size();
	const size_t size = std::max(aSize, bSize);

	out.resize(size + 1, 0);

	for (size_t i = 0; i < size; ++i)
	{
		if (i < aSize)
		{
			out[i] += a[i];
		}

		if (i < bSize)
		{
			out[i] += b[i];
		}

		out[i + 1] += out[i] / BASE;
		out[i] %= BASE;
	}
}

void BigInteger::Subtract(const Digits& a, const Digits& b, Digits& out)
{
	const size_t aSize = a.size();
	const size_t bSize = b.size();

	out.resize(aSize, 0);
	int borrow = 0;

	for (size_t i = 0; i < aSize; ++i)
	{
		std::int64_t diff = static_cast<std::int64_t>(a[i]) - (i < bSize ? static_cast<std::int64_t>(b[i]) : 0) - borrow;

		if (diff < 0)
		{
			diff += BASE;
			borrow = 1;
		}
		else
		{
			borrow = 0;
		}

		out[i] = static_cast<std::uint32_t>(diff);
	}
}

void BigInteger::Multiply(const Digits& a, const Digits& b, Digits& out)
{
	const size_t aSize = a.size();
	const size_t bSize = b.size();

	out.resize(aSize + bSize, 0);

	for (size_t i = 0; i < aSize; ++i)
	{
		std::uint64_t carry = 0;

		for (size_t j = 0; j < bSize; ++j)
		{
			std::uint64_t result = static_cast<std::uint64_t>(a[i]) * b[j] + out[i + j] + carry;

			carry = result / BASE;

			out[i + j] = static_cast<std::uint32_t>(result % BASE);
		}

		out[i + bSize] += static_cast<std::uint32_t>(carry);
	}
}

int BigInteger::CompareMagnitude(const Digits& a, const Digits& b)
{
	const size_t aSize = a.size();
	const size_t bSize = b.size();

	if (aSize != bSize)
	{
		return aSize > bSize ? 1 : -1;
	}

	for (long long i = static_cast<long long>(aSize) - 1; i >= 0; --i)
	{
		if (a[i] != b[i])
		{
			return a[i] > b[i] ? 1 : -1;
		}
	}

	return 0;
}

int BigInteger::GetDigitCount(std::uint32_t number)
{
	if (number >= 100000000)
	{
		return 9;
	}
	if (number >= 10000000)
	{
		return 8;
	}
	if (number >= 1000000)
	{
		return 7;
	}
	if (number >= 100000)
	{
		return 6;
	}
	if (number >= 10000)
	{
		return 5;
	}
	if (number >= 1000)
	{
		return 4;
	}
	if (number >= 100)
	{
		return 3;
	}
	if (number >= 10)
	{
		return 2;
	}
	return 1;
}