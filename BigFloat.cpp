#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <stdexcept>
#include "BigFloat.h"


BigFloat::BigFloat(const std::string& numStr) {
    size_t pointPos = numStr.find('.');
    std::string intPartStr = numStr.substr(0, pointPos);
    std::string fracPartStr = pointPos == std::string::npos ? "" : numStr.substr(pointPos + 1);

    integerPart = BigInt(intPartStr);

    if (fracPartStr.size() > precision) {
        fracPartStr = fracPartStr.substr(0, precision);
    }
    while (fracPartStr.size() < precision) {
        fracPartStr += '0';
    }

    fractionalPart = BigInt(fracPartStr);
}

void BigFloat::print() const {
    integerPart.print();
    std::cout << ".";

    std::string fracPartStr = fractionalPart.toString();
    while (fracPartStr.size() < precision) {
        fracPartStr = '0' + fracPartStr;
    }
    std::cout << fracPartStr.substr(0, precision) << std::endl;
}

void BigFloat::alignFractionalParts(BigFloat& a, BigFloat& b) const {
    while (a.fractionalPart.countDigits() < precision) {
        a.fractionalPart = a.fractionalPart * BigInt("10");
    }
    while (b.fractionalPart.countDigits() < precision) {
        b.fractionalPart = b.fractionalPart * BigInt("10");
    }
}

BigFloat BigFloat::add(const BigFloat& other) const {
    BigFloat result;
    BigFloat a = *this;
    BigFloat b = other;
    alignFractionalParts(a, b);

    BigInt fracSum = a.fractionalPart + b.fractionalPart;
    BigInt intCarry = BigInt("0");
    if (fracSum.countDigits() > precision) {
        intCarry = BigInt("1");
        fracSum = fracSum - BigInt("10").pow(BigInt(std::to_string(precision)));
    }
    result.fractionalPart = fracSum;

    result.integerPart = a.integerPart + b.integerPart + intCarry;

    return result;
}

BigFloat BigFloat::subtract(const BigFloat& other) const {
    BigFloat result;
    BigFloat a = *this;
    BigFloat b = other;
    alignFractionalParts(a, b);

    BigInt fracDiff;
    BigInt intBorrow = BigInt("0");
    if (a.fractionalPart < b.fractionalPart) {
        intBorrow = BigInt("1");
        fracDiff = (a.fractionalPart + BigInt("10").pow(BigInt(std::to_string(precision)))) - b.fractionalPart;
    }
    else {
        fracDiff = a.fractionalPart - b.fractionalPart;
    }
    result.fractionalPart = fracDiff;

    result.integerPart = a.integerPart - b.integerPart - intBorrow;

    return result;
}

BigFloat BigFloat::operator+(const BigFloat& other) const {
    return add(other);
}

BigFloat BigFloat::operator-(const BigFloat& other) const {
    return subtract(other);
}

BigFloat BigFloat::operator*(const BigFloat& other) const {
    BigFloat result;
    BigInt qwe;
    BigInt intPartProduct = integerPart * other.integerPart;

    // ��������� ������� ������ � ������ precision
    BigInt fracPartProduct = (fractionalPart * other.fractionalPart).divide(BigInt("10").pow(BigInt(std::to_string(precision))), qwe);

    // ��������� ��������� ������
    BigInt mixedPartProduct1 = integerPart * other.fractionalPart;
    BigInt mixedPartProduct2 = fractionalPart * other.integerPart;

    // ������������ ��������� ��������� ������
    BigInt mixedPartSum = mixedPartProduct1 + mixedPartProduct2;

    // ������� �� ������� ����� � �����
    BigInt carryFromMixed = mixedPartSum.divide(BigInt("10").pow(BigInt(std::to_string(precision))), qwe);
    result.integerPart = intPartProduct + carryFromMixed;

    // ������� �� ��������� ������ � ������� �����
    result.fractionalPart = mixedPartSum.mod(BigInt("10").pow(BigInt(std::to_string(precision)))) + fracPartProduct;

    // �������� � ������� �� ������� ����� � ����� �����
    BigInt carryFromFractional = result.fractionalPart.divide(BigInt("10").pow(BigInt(std::to_string(precision))), qwe);
    result.integerPart = result.integerPart + carryFromFractional;
    result.fractionalPart = result.fractionalPart - carryFromFractional * BigInt("10").pow(BigInt(std::to_string(precision)));

    return result;
}

//BigFloat BigFloat::operator/(const BigFloat& other) const {
//    if (other.integerPart == BigInt("0") && other.fractionalPart == BigInt("0")) {
//        throw std::invalid_argument("Division by zero");
//    }
//
//    // �������������� �������� ����� � �������� � ����� ����� ����� ���������������
//    BigInt thisScaled = integerPart * BigInt("10").pow(BigInt(std::to_string(precision))) + fractionalPart;
//    BigInt otherScaled = other.integerPart * BigInt("10").pow(BigInt(std::to_string(precision))) + other.fractionalPart;
//
//    // ���������� �������������� ������� � ����������� ���������
//    BigInt quotient = (thisScaled * BigInt("10").pow(BigInt(std::to_string(precision)))) / otherScaled;
//
//    // �������������� ���������� ������� � ������ BigFloat
//    BigFloat result;
//    result.integerPart = quotient / BigInt("10").pow(BigInt(std::to_string(precision)));
//    result.fractionalPart = quotient % BigInt("10").pow(BigInt(std::to_string(precision)));
//
//    return result;
//}
BigFloat BigFloat::operator/(const BigFloat& other) const {
    if (other.integerPart == BigInt("0") && other.fractionalPart == BigInt("0")) {
        throw std::invalid_argument("Division by zero");
    }

    // �������������� �������� ����� � �������� � ����� ����� ����� ���������������
    BigInt thisScaled = integerPart * BigInt("10").pow(BigInt(std::to_string(precision))) + fractionalPart;
    BigInt otherScaled = other.integerPart * BigInt("10").pow(BigInt(std::to_string(precision))) + other.fractionalPart;

    // ���������� ��������
    BigInt thisScaledHighPrecision = thisScaled * BigInt("10").pow(BigInt(std::to_string(precision)));

    // ���������� �������������� ������� � ����������� ���������
    BigInt quotient = thisScaledHighPrecision / otherScaled;

    // �������������� ���������� ������� � ������ BigFloat
    BigFloat result;
    result.integerPart = quotient / BigInt("10").pow(BigInt(std::to_string(precision)));
    result.fractionalPart = quotient % BigInt("10").pow(BigInt(std::to_string(precision)));

    return result;
}

