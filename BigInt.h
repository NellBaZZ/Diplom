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
#include <sstream>

class BigInt {
public:
    std::vector<uint32_t> digits;

    BigInt() = default; // конструктор по дефолту
    BigInt(const std::string& numStr); // конструктор предствалящий число строкой
    BigInt(std::vector<uint32_t>::const_iterator begin, std::vector<uint32_t>::const_iterator end); // конструктор, который инициализирует класс диапазоном элементов вектора

    // функции
    void print() const;
    BigInt multiply(const BigInt& other) const;
    BigInt divide(const BigInt& divisor, BigInt& remainder) const;
    BigInt pow(const BigInt& exponent) const;
	BigInt powmod(const BigInt& exponent, const BigInt& mod) const;
    BigInt mod(const BigInt& other) const;
    size_t countDigits() const;

    std::string toString() const; // Добавление метода toString

    // арифметические операторы
    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;
    BigInt operator/(const BigInt& divisor) const;
    BigInt operator%(const BigInt& other) const;
    BigInt operator^(const BigInt& other) const;

    // операторы сравнения
    bool operator>(const BigInt& other) const;
    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;
    bool operator<(const BigInt& other) const;
    void removeLeadingZeros();

	inline BigInt& operator+=(const BigInt& r) {return *this = *this + r;}
	inline BigInt& operator++() {return *this += BigInt("1");}
	inline BigInt operator++(int) {auto old = *this; *this += BigInt("1"); return old;}
	inline BigInt& operator-=(const BigInt& r) {return *this = *this + r;}
	inline BigInt& operator--() {return *this -= BigInt("1");}
	inline BigInt operator--(int) {auto old = *this; *this -= BigInt("1"); return old;}
	inline BigInt& operator*=(const BigInt& r) {return *this = *this * r;}
	inline BigInt& operator/=(const BigInt& r) {return *this = *this / r;}
	inline BigInt& operator%=(const BigInt& r) {return *this = *this % r;}
private:
   // void removeLeadingZeros();
  
};

BigInt add(const BigInt& a, const BigInt& b);
BigInt addSimple(const BigInt& a, const BigInt& b);
BigInt subtract(const BigInt& a, const BigInt& b); 
BigInt subtractSimple(const BigInt& a, const BigInt& b);
BigInt shift_left(const BigInt& num, size_t n); // сдвиг числа влево на n позиций
BigInt karatsuba(const BigInt& x, const BigInt& y);

inline BigInt operator""_bi(const char* val) {
	return BigInt(val);
}

#endif // BIGINT_H