#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <iostream>

class BigInteger
{
private:
	using Digits = std::vector<std::uint32_t>;

	Digits m_digits;
	bool m_isNegative;

	static constexpr std::uint64_t BASE = 1000000000ULL;
	static constexpr std::uint64_t POW_TEN[9]{
		1ULL,
		10ULL,
		100ULL,
		1000ULL,
		10000ULL,
		100000ULL,
		1000000ULL,
		10000000ULL,
		100000000ULL
	};
	static constexpr const char* ZERO_PAD_STRINGS[9]{
		"00000000",
		"0000000",
		"000000",
		"00000",
		"0000",
		"000",
		"00",
		"0",
		""
	};

public:
	BigInteger();
	BigInteger(std::int32_t number);
	BigInteger(std::uint32_t number);
	BigInteger(std::int64_t number);
	BigInteger(std::uint64_t number);
	BigInteger(const std::string& number);
	BigInteger(const BigInteger& other);
	BigInteger& operator=(const BigInteger& other);
	BigInteger(BigInteger&& other) noexcept;
	BigInteger& operator=(BigInteger&& other) noexcept;

private:
	BigInteger(Digits&& digits, bool isNegative);
	BigInteger(const Digits& digits, bool isNegative);

public:
	BigInteger operator+(const BigInteger& other) const;
	BigInteger operator+(std::int32_t other) const;
	BigInteger operator+(std::uint32_t other) const;
	BigInteger operator+(std::int64_t other) const;
	BigInteger operator+(std::uint64_t other) const;

	BigInteger operator-(const BigInteger& other) const;
	BigInteger operator-(std::int32_t other) const;
	BigInteger operator-(std::uint32_t other) const;
	BigInteger operator-(std::int64_t other) const;
	BigInteger operator-(std::uint64_t other) const;

	BigInteger operator*(const BigInteger& other) const;
	BigInteger operator*(std::int32_t other) const;
	BigInteger operator*(std::uint32_t other) const;
	BigInteger operator*(std::int64_t other) const;
	BigInteger operator*(std::uint64_t other) const;

	BigInteger operator/(const BigInteger& other) const;
	BigInteger operator/(std::int32_t other) const;
	BigInteger operator/(std::uint32_t other) const;
	BigInteger operator/(std::int64_t other) const;
	BigInteger operator/(std::uint64_t other) const;

	BigInteger operator%(const BigInteger& other) const;
	BigInteger operator%(std::int32_t other) const;
	BigInteger operator%(std::uint32_t other) const;
	BigInteger operator%(std::int64_t other) const;
	BigInteger operator%(std::uint64_t other) const;

	BigInteger& operator+=(const BigInteger& other);
	BigInteger& operator+=(std::int32_t other);
	BigInteger& operator+=(std::uint32_t other);
	BigInteger& operator+=(std::int64_t other);
	BigInteger& operator+=(std::uint64_t other);

	BigInteger& operator-=(const BigInteger& other);
	BigInteger& operator-=(std::int32_t other);
	BigInteger& operator-=(std::uint32_t other);
	BigInteger& operator-=(std::int64_t other);
	BigInteger& operator-=(std::uint64_t other);

	BigInteger& operator*=(const BigInteger& other);
	BigInteger& operator*=(std::int32_t other);
	BigInteger& operator*=(std::uint32_t other);
	BigInteger& operator*=(std::int64_t other);
	BigInteger& operator*=(std::uint64_t other);

	BigInteger& operator/=(const BigInteger& other);
	BigInteger& operator/=(std::int32_t other);
	BigInteger& operator/=(std::uint32_t other);
	BigInteger& operator/=(std::int64_t other);
	BigInteger& operator/=(std::uint64_t other);

	BigInteger& operator%=(const BigInteger& other);
	BigInteger& operator%=(std::int32_t other);
	BigInteger& operator%=(std::uint32_t other);
	BigInteger& operator%=(std::int64_t other);
	BigInteger& operator%=(std::uint64_t other);

	BigInteger operator-() const;

	bool operator<(const BigInteger& other) const;
	bool operator<(std::int32_t other) const;
	bool operator<(std::uint32_t other) const;
	bool operator<(std::int64_t other) const;
	bool operator<(std::uint64_t other) const;

	bool operator>(const BigInteger& other) const;
	bool operator>(std::int32_t other) const;
	bool operator>(std::uint32_t other) const;
	bool operator>(std::int64_t other) const;
	bool operator>(std::uint64_t other) const;

	bool operator<=(const BigInteger& other) const;
	bool operator<=(std::int32_t other) const;
	bool operator<=(std::uint32_t other) const;
	bool operator<=(std::int64_t other) const;
	bool operator<=(std::uint64_t other) const;

	bool operator>=(const BigInteger& other) const;
	bool operator>=(std::int32_t other) const;
	bool operator>=(std::uint32_t other) const;
	bool operator>=(std::int64_t other) const;
	bool operator>=(std::uint64_t other) const;

	bool operator==(const BigInteger& other) const;
	bool operator==(std::int32_t other) const;
	bool operator==(std::uint32_t other) const;
	bool operator==(std::int64_t other) const;
	bool operator==(std::uint64_t other) const;

	bool operator!=(const BigInteger& other) const;
	bool operator!=(std::int32_t other) const;
	bool operator!=(std::uint32_t other) const;
	bool operator!=(std::int64_t other) const;
	bool operator!=(std::uint64_t other) const;

private:
	bool IsValid(const std::string& number);
	void Normalize();

public:
	std::string ToString() const;
	BigInteger Abs() const;

private:
	static void Add(const Digits& a, const Digits& b, Digits& out);
	static void Subtract(const Digits& a, const Digits& b, Digits& out);
	static void Multiply(const Digits& a, const Digits& b, Digits& out);
	static void MultiplyByDigit(const Digits& a, std::uint32_t digit, Digits& out);
	static void Divide(const Digits& dividend, const Digits& divisor, Digits& quotient, Digits& remainder);

	// return 1 when a > b, return 0 when a == b, return -1 when a < b
	static int CompareMagnitude(const Digits& a, const Digits& b);
	static int GetDigitCount(std::uint32_t number);
	static void NormalizeDigits(Digits& digits);
};

std::ostream& operator<<(std::ostream& os, const BigInteger& num);