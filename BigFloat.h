#pragma once
#include "BigInt.h"

class BigFloat {
public:
    BigInt integerPart;
    BigInt fractionalPart;
    static const int precision = 51;

    BigFloat() = default;
    BigFloat(const std::string& numStr);

    void print() const;

    BigFloat operator+(const BigFloat& other) const;
    BigFloat operator-(const BigFloat& other) const;
    BigFloat operator*(const BigFloat& other) const;
    BigFloat operator/(const BigFloat& other) const;

private:
    void alignFractionalParts(BigFloat& a, BigFloat& b) const;
    BigFloat add(const BigFloat& other) const;
    BigFloat subtract(const BigFloat& other) const;
};