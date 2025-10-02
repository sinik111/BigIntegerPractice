#pragma once

#include <vector>
#include <cstdint>
#include <string>

class BigInteger
{
private:
	std::vector<std::uint32_t> m_digits;
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
	BigInteger(std::vector<std::uint32_t>&& digits, bool isNegative);

public:
	BigInteger operator+(const BigInteger& other);
	BigInteger operator-(const BigInteger& other);



	bool operator<(const BigInteger& other) const;
	bool operator>(const BigInteger& other) const;
	bool operator<=(const BigInteger& other) const;
	bool operator>=(const BigInteger& other) const;
	bool operator==(const BigInteger& other) const;
	bool operator!=(const BigInteger& other) const;
};