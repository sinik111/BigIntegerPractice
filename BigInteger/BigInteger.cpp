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
	Digits temp;
	bool negative = false;

	if (m_isNegative == other.m_isNegative)
	{
		negative = m_isNegative;

		Add(m_digits, other.m_digits, temp);
	}
	else
	{
		if (CompareMagnitude(m_digits, other.m_digits) > 0)
		{
			negative = m_isNegative;

			Subtract(m_digits, other.m_digits, temp);
		}
		else
		{
			negative = !m_isNegative;

			Subtract(other.m_digits, m_digits, temp);
		}
	}

	BigInteger result = BigInteger(std::move(temp), negative);
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
	Digits temp;
	bool negative = false;

	if (m_isNegative == other.m_isNegative)
	{
		if (CompareMagnitude(m_digits, other.m_digits) > 0)
		{
			negative = m_isNegative;

			Subtract(m_digits, other.m_digits, temp);
		}
		else
		{
			negative = !m_isNegative;

			Subtract(other.m_digits, m_digits, temp);
		}
	}
	else
	{
		negative = m_isNegative;

		Add(m_digits, other.m_digits, temp);
	}

	BigInteger result = BigInteger(std::move(temp), negative);
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
	Digits temp;

	Multiply(m_digits, other.m_digits, temp);

	BigInteger result = BigInteger(std::move(temp), m_isNegative != other.m_isNegative);
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

BigInteger BigInteger::operator/(const BigInteger& other) const
{
	Digits quotient;
	Digits remain;

	Divide(m_digits, other.m_digits, quotient, remain);

	BigInteger result = BigInteger(std::move(quotient), m_isNegative != other.m_isNegative);
	result.Normalize();

	// 백준 큰 수 계산용
	if (m_isNegative != other.m_isNegative && !(remain.size() == 1 && remain[0] == 0))
	{
		result -= 1;
	}
	result.Normalize();

	return result;
}

BigInteger BigInteger::operator/(std::int32_t other) const
{
	return *this / BigInteger(other);
}

BigInteger BigInteger::operator/(std::uint32_t other) const
{
	return *this / BigInteger(other);
}

BigInteger BigInteger::operator/(std::int64_t other) const
{
	return *this / BigInteger(other);
}

BigInteger BigInteger::operator/(std::uint64_t other) const
{
	return *this / BigInteger(other);
}

BigInteger BigInteger::operator%(const BigInteger& other) const
{
	Digits quotient;
	Digits remain;

	Divide(m_digits, other.m_digits, quotient, remain);

	BigInteger result = BigInteger(std::move(remain), m_isNegative != other.m_isNegative);
	result.Normalize();

	return result;
}

BigInteger BigInteger::operator%(std::int32_t other) const
{
	return *this % BigInteger(other);
}

BigInteger BigInteger::operator%(std::uint32_t other) const
{
	return *this % BigInteger(other);
}

BigInteger BigInteger::operator%(std::int64_t other) const
{
	return *this % BigInteger(other);
}

BigInteger BigInteger::operator%(std::uint64_t other) const
{
	return *this % BigInteger(other);
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

BigInteger& BigInteger::operator/=(const BigInteger& other)
{
	*this = *this / other;
	return *this;
}

BigInteger& BigInteger::operator/=(std::int32_t other)
{
	*this = *this / other;
	return *this;
}

BigInteger& BigInteger::operator/=(std::uint32_t other)
{
	*this = *this / other;
	return *this;
}

BigInteger& BigInteger::operator/=(std::int64_t other)
{
	*this = *this / other;
	return *this;
}

BigInteger& BigInteger::operator/=(std::uint64_t other)
{
	*this = *this / other;
	return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger& other)
{
	*this = *this % other;
	return *this;
}

BigInteger& BigInteger::operator%=(std::int32_t other)
{
	*this = *this % other;
	return *this;
}

BigInteger& BigInteger::operator%=(std::uint32_t other)
{
	*this = *this % other;
	return *this;
}

BigInteger& BigInteger::operator%=(std::int64_t other)
{
	*this = *this % other;
	return *this;
}

BigInteger& BigInteger::operator%=(std::uint64_t other)
{
	*this = *this % other;
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

bool BigInteger::operator<(std::int32_t other) const
{
	return *this < BigInteger(other);
}

bool BigInteger::operator<(std::uint32_t other) const
{
	return *this < BigInteger(other);
}

bool BigInteger::operator<(std::int64_t other) const
{
	return *this < BigInteger(other);
}

bool BigInteger::operator<(std::uint64_t other) const
{
	return *this < BigInteger(other);
}

bool BigInteger::operator>(const BigInteger& other) const
{
	return !(*this == other) && !(*this < other);
}

bool BigInteger::operator>(std::int32_t other) const
{
	return *this > BigInteger(other);
}

bool BigInteger::operator>(std::uint32_t other) const
{
	return *this > BigInteger(other);
}

bool BigInteger::operator>(std::int64_t other) const
{
	return *this > BigInteger(other);
}

bool BigInteger::operator>(std::uint64_t other) const
{
	return *this > BigInteger(other);
}

bool BigInteger::operator<=(const BigInteger& other) const
{
	return (*this == other) || (*this < other);
}

bool BigInteger::operator<=(std::int32_t other) const
{
	return *this <= BigInteger(other);
}

bool BigInteger::operator<=(std::uint32_t other) const
{
	return *this <= BigInteger(other);
}

bool BigInteger::operator<=(std::int64_t other) const
{
	return *this <= BigInteger(other);
}

bool BigInteger::operator<=(std::uint64_t other) const
{
	return *this <= BigInteger(other);
}

bool BigInteger::operator>=(const BigInteger& other) const
{
	return (*this == other) || !(*this < other);
}

bool BigInteger::operator>=(std::int32_t other) const
{
	return *this >= BigInteger(other);
}

bool BigInteger::operator>=(std::uint32_t other) const
{
	return *this >= BigInteger(other);
}

bool BigInteger::operator>=(std::int64_t other) const
{
	return *this >= BigInteger(other);
}

bool BigInteger::operator>=(std::uint64_t other) const
{
	return *this >= BigInteger(other);
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

bool BigInteger::operator==(std::int32_t other) const
{
	return *this == BigInteger(other);
}

bool BigInteger::operator==(std::uint32_t other) const
{
	return *this == BigInteger(other);
}

bool BigInteger::operator==(std::int64_t other) const
{
	return *this == BigInteger(other);
}

bool BigInteger::operator==(std::uint64_t other) const
{
	return *this == BigInteger(other);
}

bool BigInteger::operator!=(const BigInteger& other) const
{
	return !(*this == other);
}

bool BigInteger::operator!=(std::int32_t other) const
{
	return *this != BigInteger(other);
}

bool BigInteger::operator!=(std::uint32_t other) const
{
	return *this != BigInteger(other);
}

bool BigInteger::operator!=(std::int64_t other) const
{
	return *this != BigInteger(other);
}

bool BigInteger::operator!=(std::uint64_t other) const
{
	return *this != BigInteger(other);
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
	out.clear();

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

void BigInteger::Subtract(const Digits& bigger, const Digits& smaller, Digits& out)
{
	out.clear();

	const size_t biggerSize = bigger.size();
	const size_t smallerSize = smaller.size();

	out.resize(biggerSize, 0);
	int borrow = 0;

	for (size_t i = 0; i < biggerSize; ++i)
	{
		std::int64_t diff = static_cast<std::int64_t>(bigger[i]) -
			(i < smallerSize ? static_cast<std::int64_t>(smaller[i]) : 0) - borrow;

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
	// todo : FFT(또는 NTT)로 구현

	out.clear();

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

void BigInteger::MultiplyByDigit(const Digits& a, std::uint32_t digit, Digits& out)
{
	// todo : FFT(또는 NTT)로 구현

	out.clear();

	if (digit == 0)
	{
		out.push_back(0);

		return;
	}

	std::uint64_t carry = 0;

	for (size_t i = 0; i < a.size(); ++i)
	{
		std::uint64_t result = static_cast<std::uint64_t>(a[i]) * digit + carry;
		
		carry = result / BASE;

		out.push_back(static_cast<std::uint32_t>(result % BASE));
	}

	if (carry > 0)
	{
		out.push_back(static_cast<std::uint32_t>(carry));
	}
}

void BigInteger::Divide(const Digits& dividend, const Digits& divisor, Digits& quotient, Digits& remain)
{
	// todo : Knuth Algorithm D 로 구현

	quotient.clear();
	remain.clear();

	if (divisor.size() == 1 && divisor[0] == 0)
	{
		assert(false);

		quotient.push_back(0);

		return;
	}

	int diff = CompareMagnitude(dividend, divisor);

	if (diff == -1)
	{
		quotient.push_back(0);
		remain = dividend;

		return;
	}

	if (diff == 0)
	{
		quotient.push_back(1);
		remain.push_back(0);

		return;
	}

	for (int i = static_cast<int>(dividend.size()) - 1; i >= 0; --i)
	{
		remain.insert(remain.begin(), dividend[i]);
		NormalizeDigits(remain);

		if (CompareMagnitude(remain, divisor) < 0)
		{
			quotient.insert(quotient.begin(), 0);

			continue;
		}

		std::uint32_t left = 1;
		std::uint32_t right = BASE - 1;
		std::uint32_t qDigit = 0;

		while (left <= right)
		{
			std::uint32_t mid = left + (right - left) / 2;

			Digits temp;
			MultiplyByDigit(divisor, mid, temp);

			if (CompareMagnitude(temp, remain) <= 0)
			{
				qDigit = mid;
				left = mid + 1;
			}
			else
			{
				right = mid - 1;
			}
		}

		Digits multiplyTemp, subtractTemp;
		MultiplyByDigit(divisor, qDigit, multiplyTemp);
		Subtract(remain, multiplyTemp, subtractTemp);

		remain = std::move(subtractTemp);

		quotient.insert(quotient.begin(), qDigit);
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

void BigInteger::NormalizeDigits(Digits& digits)
{
	while (digits.size() > 1 && digits.back() == 0)
	{
		digits.pop_back();
	}

	if (digits.empty())
	{
		digits.push_back(0);
	}
}

std::ostream& operator<<(std::ostream& os, const BigInteger& num)
{
	os << num.ToString();

	return os;
}