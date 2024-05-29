#pragma once
#ifndef BIGINT_H
#define BIGINT_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cstdint>
#include <cmath>
#include <chrono>
#include <immintrin.h>

class BigInt {
public:
    std::vector<uint32_t> digits;

    BigInt() = default;
    BigInt(const std::string& numStr);
    BigInt(std::vector<uint32_t>::const_iterator begin, std::vector<uint32_t>::const_iterator end);


    void print() const;
    BigInt multiply(const BigInt& other) const;
    BigInt pow(const BigInt& exponent) const;
    size_t countDigits() const;

    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;
    BigInt operator/(const BigInt& divisor) const;
    BigInt operator%(const BigInt& other) const;
    BigInt operator^(const BigInt& other) const;

    bool operator>(const BigInt& other) const;
    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;
    bool operator<(const BigInt& other) const;

private:
    void removeLeadingZeros();
  
};

BigInt add(const BigInt& a, const BigInt& b);
BigInt addSimple(const BigInt& a, const BigInt& b);
BigInt subtract(const BigInt& a, const BigInt& b);
BigInt shift_left(const BigInt& num, size_t n);
BigInt karatsuba(const BigInt& x, const BigInt& y);

#endif // BIGINT_H