//#include <iostream>
//#include <vector>
//#include <string>
//#include <algorithm>
//#include <iomanip>
//#include <cstdint>
//#include <cmath>
//#include <chrono>
//#include <immintrin.h>
//
//class BigInt {
//public:
//    std::vector<uint32_t> digits;
//
//    BigInt() = default;
//    BigInt(const std::string& numStr);
//    BigInt(std::vector<uint32_t>::const_iterator begin, std::vector<uint32_t>::const_iterator end);
//
//    void print() const;
//    BigInt multiply(const BigInt& other) const;
//    BigInt pow(const BigInt& exponent) const;
//    size_t countDigits() const;
//
//    BigInt operator*(const BigInt& other) const {
//        return multiply(other);
//    }
//
//    bool operator>(const BigInt& other) const;
//    bool operator==(const BigInt& other) const;
//    bool operator!=(const BigInt& other) const;
//    bool operator<=(const BigInt& other) const;
//    bool operator>=(const BigInt& other) const;
//    bool operator<(const BigInt& other) const;
//    BigInt operator/(const BigInt& divisor) const;
//    BigInt operator-(const BigInt& other) const;
//    BigInt operator%(const BigInt& other) const;
//
//private:
//    void removeLeadingZeros();
//};
//
//// Конструктор для инициализации BigInt из строки
//
//BigInt::BigInt(const std::string& numStr) {
//    for (int i = numStr.size(); i > 0; i -= 9) {
//        if (i < 9) {
//            digits.push_back(static_cast<uint32_t>(std::stoul(numStr.substr(0, i))));
//        }
//        else {
//            digits.push_back(static_cast<uint32_t>(std::stoul(numStr.substr(i - 9, 9))));
//        }
//    }
//}
//
//BigInt::BigInt(std::vector<uint32_t>::const_iterator begin, std::vector<uint32_t>::const_iterator end) {
//    digits.assign(begin, end);
//}
//
//void BigInt::print() const {
//    bool first = true;
//    for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
//        if (first) {
//            std::cout << *it;
//            first = false;
//        }
//        else {
//            std::cout << std::setw(9) << std::setfill('0') << *it;
//        }
//    }
//    std::cout << std::endl;
//}
//
//size_t BigInt::countDigits() const {
//    if (digits.empty()) {
//        return 0;
//    }
//
//    size_t count = 0;
//    for (size_t i = 0; i < digits.size(); ++i) {
//        if (digits[i] == 0) {
//            count += 1;
//        }
//        else {
//            count += static_cast<size_t>(std::log10(digits[i])) + 1;
//        }
//    }
//
//    return count;
//}
//
//bool BigInt::operator<(const BigInt& other) const {
//    if (digits.size() != other.digits.size()) {
//        return digits.size() < other.digits.size();
//    }
//    for (size_t i = digits.size(); i > 0; --i) {
//        if (digits[i - 1] != other.digits[i - 1]) {
//            return digits[i - 1] < other.digits[i - 1];
//        }
//    }
//    return false;
//}
//
//bool BigInt::operator>(const BigInt& other) const {
//    if (digits.size() != other.digits.size()) {
//        return digits.size() > other.digits.size();
//    }
//    for (size_t i = digits.size(); i > 0; --i) {
//        if (digits[i - 1] != other.digits[i - 1]) {
//            return digits[i - 1] > other.digits[i - 1];
//        }
//    }
//    return false;
//}
//
//bool BigInt::operator==(const BigInt& other) const {
//    return digits == other.digits;
//}
//
//bool BigInt::operator!=(const BigInt& other) const {
//    return !(*this == other);
//}
//
//bool BigInt::operator<=(const BigInt& other) const {
//    return !(*this > other);
//}
//
//bool BigInt::operator>=(const BigInt& other) const {
//    return !(*this < other);
//}
//
//BigInt BigInt::operator/(const BigInt& divisor) const {
//    BigInt result, current;
//    for (size_t i = digits.size(); i > 0; --i) {
//        current.digits.insert(current.digits.begin(), digits[i - 1]);
//        current.removeLeadingZeros();
//        uint32_t x = 0, l = 0, r = 1000000000;
//        while (l <= r) {
//            uint32_t m = (l + r) / 2;
//            BigInt t = divisor * BigInt(std::to_string(m));
//            if (t <= current) {
//                x = m;
//                l = m + 1;
//            }
//            else {
//                r = m - 1;
//            }
//        }
//        result.digits.insert(result.digits.begin(), x);
//        current = current - (divisor * BigInt(std::to_string(x)));
//    }
//    result.removeLeadingZeros();
//    return result;
//}
//
//BigInt BigInt::operator-(const BigInt& other) const {
//    BigInt result = *this;
//    int64_t carry = 0;
//    for (size_t i = 0; i < other.digits.size() || carry != 0; ++i) {
//        result.digits[i] -= carry + (i < other.digits.size() ? other.digits[i] : 0);
//        if (result.digits[i] < 0) {
//            carry = 1;
//            result.digits[i] += 1000000000;
//        }
//        else {
//            carry = 0;
//        }
//    }
//    result.removeLeadingZeros();
//    return result;
//}
//
//BigInt BigInt::operator%(const BigInt& divisor) const {
//    BigInt result = *this;
//    while (result >= divisor) {
//        result = result - divisor;
//    }
//    return result;
//}
//
//void BigInt::removeLeadingZeros() {
//    while (digits.size() > 1 && digits.back() == 0) {
//        digits.pop_back();
//    }
//}
//
//// Сложение
//
//
//
//BigInt add(const BigInt& a, const BigInt& b) {
//    BigInt result;
//    size_t maxSize = std::max(a.digits.size(), b.digits.size());
//    result.digits.resize(maxSize + 1);
//
//    size_t avxSize = maxSize / 16; // AVX-512 обрабатывает 16 32-битных элементов за раз
//    size_t i = 0;
//    __m512i carry = _mm512_setzero_si512();
//
//    // Убедимся, что оба числа имеют одинаковое количество цифр
//    std::vector<uint32_t> a_digits = a.digits;
//    std::vector<uint32_t> b_digits = b.digits;
//
//    if (a.digits.size() < maxSize) {
//        a_digits.resize(maxSize, 0);
//    }
//    if (b.digits.size() < maxSize) {
//        b_digits.resize(maxSize, 0);
//    }
//
//    for (; i < avxSize * 16; i += 16) {
//        __m512i va = _mm512_loadu_si512(reinterpret_cast<const __m512i*>(&a_digits[i]));
//        __m512i vb = _mm512_loadu_si512(reinterpret_cast<const __m512i*>(&b_digits[i]));
//        __m512i vr = _mm512_add_epi32(va, vb);
//        vr = _mm512_add_epi32(vr, carry);
//
//        __mmask16 carry_mask = _mm512_cmp_epu32_mask(vr, va, _MM_CMPINT_LT);
//        carry = _mm512_maskz_set1_epi32(carry_mask, 1);
//
//        _mm512_storeu_si512(reinterpret_cast<__m512i*>(&result.digits[i]), vr);
//    }
//
//    uint32_t carry_scalar = _mm512_reduce_add_epi32(carry) & 1; // Суммируем переносы и берем младший бит
//
//    // Обработка оставшихся элементов
//    for (; i < maxSize; ++i) {
//        uint32_t digitA = (i < a.digits.size()) ? a.digits[i] : 0;
//        uint32_t digitB = (i < b.digits.size()) ? b.digits[i] : 0;
//
//        uint64_t sum = static_cast<uint64_t>(digitA) + digitB + carry_scalar;
//        carry_scalar = sum >> 32; // Перенос
//        result.digits[i] = static_cast<uint32_t>(sum);
//    }
//
//    if (carry_scalar) {
//        result.digits[maxSize] = carry_scalar;
//    }
//    else {
//        result.digits.pop_back();
//    }
//
//    while (result.digits.size() > 1 && result.digits.back() == 0) {
//        result.digits.pop_back();
//    }
//
//
//    return result;
//}
//
//// вычитание
//BigInt subtract(const BigInt& a, const BigInt& b) {
//    BigInt result;
//    size_t maxSize = a.digits.size();
//    result.digits.resize(maxSize);
//
//    size_t avxSize = maxSize / 16; // AVX-512 обрабатывает 16 32-битных элементов за раз
//    size_t i = 0;
//    __m512i borrow = _mm512_setzero_si512();
//
//    // Убедимся, что оба числа имеют одинаковое количество цифр
//    std::vector<uint32_t> a_digits = a.digits;
//    std::vector<uint32_t> b_digits = b.digits;
//
//    if (b.digits.size() < maxSize) {
//        b_digits.resize(maxSize, 0);
//    }
//
//    for (; i < avxSize * 16; i += 16) {
//        __m512i va = _mm512_loadu_si512(reinterpret_cast<const __m512i*>(&a_digits[i]));
//        __m512i vb = _mm512_loadu_si512(reinterpret_cast<const __m512i*>(&b_digits[i]));
//        __m512i vr = _mm512_sub_epi32(va, vb);
//        vr = _mm512_sub_epi32(vr, borrow);
//
//        __mmask16 borrow_mask = _mm512_cmp_epu32_mask(vr, va, _MM_CMPINT_GT);
//        borrow = _mm512_maskz_set1_epi32(borrow_mask, 1);
//
//        _mm512_storeu_si512(reinterpret_cast<__m512i*>(&result.digits[i]), vr);
//    }
//
//    uint32_t borrow_scalar = _mm512_reduce_add_epi32(borrow) & 1; // Суммируем заимствования и берем младший бит
//
//    // Обработка оставшихся элементов
//    for (; i < maxSize; ++i) {
//        uint32_t digitA = a.digits[i];
//        uint32_t digitB = (i < b.digits.size()) ? b.digits[i] : 0;
//
//        int64_t sub = static_cast<int64_t>(digitA) - digitB - borrow_scalar;
//        if (sub < 0) {
//            sub += (1ULL << 32);
//            borrow_scalar = 1;
//        }
//        else {
//            borrow_scalar = 0;
//        }
//
//        result.digits[i] = static_cast<uint32_t>(sub);
//    }
//
//    // Удаление ведущих нулей
//    while (result.digits.size() > 1 && result.digits.back() == 0) {
//        result.digits.pop_back();
//    }
//
//
//    return result;
//}
//BigInt BigInt::multiply(const BigInt& other) const {
//    BigInt result;
//    result.digits.resize(digits.size() + other.digits.size(), 0);
//
//    for (size_t i = 0; i < digits.size(); ++i) {
//        uint64_t carry = 0;
//        for (size_t j = 0; j < other.digits.size() || carry != 0; ++j) {
//            uint64_t current = result.digits[i + j] + static_cast<uint64_t>(digits[i]) * (j < other.digits.size() ? other.digits[j] : 0) + carry;
//            result.digits[i + j] = static_cast<uint32_t>(current % 1000000000);
//            carry = current / 1000000000;
//        }
//    }
//
//    // Удаление ведущих нулей
//    while (result.digits.size() > 1 && result.digits.back() == 0) {
//        result.digits.pop_back();
//    }
//
//    return result;
//}
//
//
//BigInt shift_left(const BigInt& num, size_t n) {
//    if (n == 0) return num;
//    BigInt result;
//    result.digits.resize(num.digits.size() + n, 0);
//    std::copy(num.digits.begin(), num.digits.end(), result.digits.begin() + n);
//    return result;
//}
//BigInt karatsuba(const BigInt& x, const BigInt& y) {
//    size_t n = std::max(x.digits.size(), y.digits.size());
//
//    if (n <= 32) { // Порог для перехода на обычное умножение
//        return x * y;
//    }
//
//    n = (n / 2) + (n % 2); // Округление вверх для деления на 2
//
//    // Разделение x и y на две части
//    BigInt x1(x.digits.size() > n ? x.digits.begin() + n : x.digits.end(), x.digits.end());
//    BigInt x0(x.digits.begin(), x.digits.size() > n ? x.digits.begin() + n : x.digits.end());
//
//    BigInt y1(y.digits.size() > n ? y.digits.begin() + n : y.digits.end(), y.digits.end());
//    BigInt y0(y.digits.begin(), y.digits.size() > n ? y.digits.begin() + n : y.digits.end());
//
//    // Рекурсивные вычисления
//    BigInt z0 = karatsuba(x0, y0);
//    BigInt z1 = karatsuba(add(x0, x1), add(y0, y1));
//    BigInt z2 = karatsuba(x1, y1);
//
//    // Корректное объединение результатов
//    BigInt r1 = shift_left(z2, 2 * n * 10 / 10);  // сдвиг на 2*n (в цифрах, каждый элемент 10^9)
//    BigInt r2 = shift_left(subtract(subtract(z1, z2), z0), n * 10 / 10);  // сдвиг на n (в цифрах, каждый элемент 10^9)
//
//    return add(add(r1, r2), z0);
//}
//
//// Возведение в степень
//BigInt BigInt::pow(const BigInt& exponent) const {
//    BigInt base = *this;
//    BigInt exp = exponent;
//    BigInt result("1");
//
//    BigInt zero("0");
//    //BigInt one("1");
//    BigInt two("2");
//
//    while (exp > zero) {
//        if ((exp.digits[0] % 2) == 1) {  // Нечетное
//            result = karatsuba(result, base);
//            // result = result * base;
//        }
//        base = karatsuba(base, base);
//        // base = base * base;
//        exp = exp / two;  // Целочисленное деление
//    }
//
//    return result;
//}
//
//
//
//int main() {
//
//
//    auto start = std::chrono::high_resolution_clock::now();
//    BigInt a("234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
//    BigInt b("234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
//
//    //  BigInt result = a.pow(b);
//    //   BigInt result = a * b;
//    BigInt result = karatsuba(a, b);
//    result.print();
//    auto end = std::chrono::high_resolution_clock::now();
//    std::chrono::duration<double> duration = end - start;
//    std::cout << "Time taken by function: " << duration.count() << " seconds" << std::endl;
//
//}
