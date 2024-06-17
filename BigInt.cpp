#include "BigInt.h"

// Инициализация констант
BigInt zero("0");
BigInt one("1");
BigInt two("2");

// Строка делится на вектора по 9 цифр
BigInt::BigInt(const std::string& numStr) {
    for (int i = numStr.size(); i > 0; i -= 9) {
        if (i < 9) {
            digits.push_back(static_cast<uint32_t>(std::stoul(numStr.substr(0, i))));
        }
        else {
            digits.push_back(static_cast<uint32_t>(std::stoul(numStr.substr(i - 9, 9))));
        }
    }
}

BigInt::BigInt(std::vector<uint32_t>::const_iterator begin, std::vector<uint32_t>::const_iterator end) {
    digits.assign(begin, end);
}

// принт
void BigInt::print() const {
    bool first = true;
    for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
        if (first) {
            std::cout << *it;
            first = false;
        }
        else {
            std::cout << std::setw(9) << std::setfill('0') << *it;
        }
    }
    std::cout << std::endl;
}

// подсчет кол-ва цифр
size_t BigInt::countDigits() const {
    if (digits.empty()) {
        return 0;
    }

    size_t count = 0;
    for (size_t i = 0; i < digits.size(); ++i) {
        if (digits[i] == 0) {
            count += 1;
        }
        else {
            count += static_cast<size_t>(std::log10(digits[i])) + 1;
        }
    }

    return count;
}

bool BigInt::operator<(const BigInt& other) const {
    if (digits.size() != other.digits.size()) {
        return digits.size() < other.digits.size();
    }
    for (size_t i = digits.size(); i > 0; --i) {
        if (digits[i - 1] != other.digits[i - 1]) {
            return digits[i - 1] < other.digits[i - 1];
        }
    }
    return false;
}

bool BigInt::operator>(const BigInt& other) const {
    if (digits.size() != other.digits.size()) {
        return digits.size() > other.digits.size();
    }
    for (size_t i = digits.size(); i > 0; --i) {
        if (digits[i - 1] != other.digits[i - 1]) {
            return digits[i - 1] > other.digits[i - 1];
        }
    }
    return false;
}

bool BigInt::operator==(const BigInt& other) const {
    return digits == other.digits;
}

bool BigInt::operator!=(const BigInt& other) const {
    return !(*this == other);
}

bool BigInt::operator<=(const BigInt& other) const {
    return !(*this > other);
}

bool BigInt::operator>=(const BigInt& other) const {
    return !(*this < other);
}

BigInt BigInt::operator*(const BigInt& other) const {
    return multiply(other);
}


BigInt BigInt::operator+(const BigInt& other) const {
    return add(*this, other);
}

BigInt BigInt::operator^(const BigInt& other) const {
    BigInt base = *this;
    return base.pow(other);

}

BigInt BigInt::operator-(const BigInt& other) const {
    return subtract(*this, other);
}

BigInt BigInt::operator%(const BigInt& divisor) const {
    //BigInt result = *this;
    //while (result >= divisor) {
    //    result = result - divisor;
    //}
    //return result;
	/*BigInt r;
	divide(*this, r);
	return r;*/

    BigInt quotient = *this / divisor;
    BigInt product = quotient * divisor;
    BigInt remainder = subtract(*this, product);
    return remainder;
}


void BigInt::removeLeadingZeros() {
    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
}

// Сложение - С avx-512
BigInt add(const BigInt& a, const BigInt& b) {
    if ((a.digits.size() == 0) && (b.digits.size() == 0)) {
        return BigInt(); // возвращается пустой объект
    }
    else if (a.digits.size() == 0) {
        return b; 
    }
    else if (b.digits.size() == 0) {
        return a; 
    }

    // определяется макс размер для результата
    size_t maxSize = std::max(a.digits.size(), b.digits.size());
    BigInt result;
    result.digits.resize(maxSize + 1);

    uint32_t carry = 0;
    size_t i = 0;

    // за раз обрабатывается 16 32-битных чисел
    for (; i + 15 < maxSize; i += 16) {
        __m512i v_a = _mm512_loadu_si512(reinterpret_cast<const __m512i*>(a.digits.data() + std::min(i, a.digits.size() - 16)));
        __m512i v_b = _mm512_loadu_si512(reinterpret_cast<const __m512i*>(b.digits.data() + std::min(i, b.digits.size() - 16)));
        __m512i v_c = _mm512_set1_epi32(carry);

        __m512i sum = _mm512_add_epi32(_mm512_add_epi32(v_a, v_b), v_c);

        _mm512_storeu_si512(reinterpret_cast<__m512i*>(result.digits.data() + i), sum);

        carry = 0;
        for (int j = 0; j < 16; ++j) {
            uint64_t value = result.digits[i + j] + static_cast<uint64_t>(carry);
            result.digits[i + j] = value % 1000000000;
            carry = value / 1000000000;
        }
    }

    // обработка оставшихся элементов, если кол-во цифр не кратно 16
    for (; i < maxSize; ++i) {
        uint64_t sum = carry;
        if (i < a.digits.size()) sum += a.digits[i];
        if (i < b.digits.size()) sum += b.digits[i];
        result.digits[i] = sum % 1000000000;
        carry = sum / 1000000000;
    }

    // eсли после завершения остался перенос, он добавляется как новый элемент в result.digits 
    //Если переноса нет, последний элемент вектора удаляется, так как он инициализировался нулём в началe

    if (carry > 0) {
        result.digits[i] = carry;
    }
    else {
        result.digits.pop_back();
    }

    // удаление ведущих нулей
    while (result.digits.size() > 1 && result.digits.back() == 0) {
        result.digits.pop_back();
    }

    return result;
}

// Сложение - Алгоритм без avx-512
BigInt addSimple(const BigInt& a, const BigInt& b) {
    BigInt result;
    size_t maxSize = std::max(a.digits.size(), b.digits.size());
    result.digits.resize(maxSize + 1);

    uint64_t carry = 0;
    for (size_t i = 0; i < maxSize || carry; ++i) {
        uint64_t sum = carry;
        if (i < a.digits.size()) sum += a.digits[i];
        if (i < b.digits.size()) sum += b.digits[i];
        result.digits[i] = sum % 1000000000;//1000000000;
        carry = sum / 1000000000;
    }

    if (result.digits.back() == 0) {
        result.digits.pop_back();
    }
    while (result.digits.size() > 1 && result.digits.back() == 0) {
        result.digits.pop_back();
    }
    return result;
}

// Вычитание - c avx-512
BigInt subtract(const BigInt& a, const BigInt& b) {
    BigInt result;
    size_t maxSize = a.digits.size();
    result.digits.resize(maxSize);

    size_t avxSize = maxSize / 16;
    size_t i = 0;
    __m512i borrow = _mm512_setzero_si512();//  вектор нулей для отслеживания заимствования

    std::vector<uint32_t> a_digits = a.digits;
    std::vector<uint32_t> b_digits = b.digits;

    if (b.digits.size() < maxSize) {// если вектор б меньше. чем а, то дополняется нулями
        b_digits.resize(maxSize, 0);
    }

    for (; i < avxSize * 16; i += 16) {
        __m512i va = _mm512_loadu_si512(reinterpret_cast<const __m512i*>(&a_digits[i]));
        __m512i vb = _mm512_loadu_si512(reinterpret_cast<const __m512i*>(&b_digits[i]));
        __m512i vr = _mm512_sub_epi32(va, vb);
        vr = _mm512_sub_epi32(vr, borrow);

        __mmask16 borrow_mask = _mm512_cmp_epu32_mask(vr, va, _MM_CMPINT_GT);
        borrow = _mm512_maskz_set1_epi32(borrow_mask, 1);

        _mm512_storeu_si512(reinterpret_cast<__m512i*>(&result.digits[i]), vr);
    }

    uint32_t borrow_scalar = _mm512_reduce_add_epi32(borrow) & 1;

    for (; i < maxSize; ++i) {
        uint32_t digitA = a.digits[i];
        uint32_t digitB = (i < b.digits.size()) ? b.digits[i] : 0;

        int64_t sub = static_cast<int64_t>(digitA) - digitB - borrow_scalar;
        if (sub < 0) {
            sub += (1ULL << 32);
            borrow_scalar = 1;
        }
        else {
            borrow_scalar = 0;
        }

        result.digits[i] = static_cast<uint32_t>(sub);
    }

    while (result.digits.size() > 1 && result.digits.back() == 0) {
        result.digits.pop_back();
    }

    return result;
}

BigInt subtractSimple(const BigInt& a, const BigInt& b) {
    BigInt result = a;
    int64_t carry = 0;

    for (size_t i = 0; i < b.digits.size() || carry != 0; ++i) {
        result.digits[i] -= carry + (i < b.digits.size() ? b.digits[i] : 0);
        if (result.digits[i] < 0) {
            carry = 1;
            result.digits[i] += 1000000000;
        }
        else {
            carry = 0;
        }
    }

    result.removeLeadingZeros();
    return result;
}

// Умножение - обычное умножение
BigInt BigInt::multiply(const BigInt& other) const {
    BigInt result;
    result.digits.resize(digits.size() + other.digits.size(), 0);

    for (size_t i = 0; i < digits.size(); ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < other.digits.size() || carry != 0; ++j) {
            uint64_t current = result.digits[i + j] + static_cast<uint64_t>(digits[i]) * (j < other.digits.size() ? other.digits[j] : 0) + carry;
            result.digits[i + j] = static_cast<uint32_t>(current % 1000000000);
            carry = current / 1000000000;
        }
    }

    while (result.digits.size() > 1 && result.digits.back() == 0) {
        result.digits.pop_back();
    }

    return result;
}

BigInt shift_left(const BigInt& num, size_t n) {
    if (n == 0) return num;
    BigInt result;
    result.digits.resize(num.digits.size() + n, 0);
    std::copy(num.digits.begin(), num.digits.end(), result.digits.begin() + n);
    return result;
}

// Умножение - Алгоритм умножения Карацубы
BigInt karatsuba(const BigInt& x, const BigInt& y) {
    if ((x == zero) || (y == zero)) return zero;
    if (x == one) { return y; }
    else if (y == one) { return x; }

    size_t n = std::max(x.digits.size(), y.digits.size());

    if (n <= 32) {
        return x * y;
    }

    n = (n / 2) + (n % 2);

    BigInt x1(x.digits.size() > n ? x.digits.begin() + n : x.digits.end(), x.digits.end());
    BigInt x0(x.digits.begin(), x.digits.size() > n ? x.digits.begin() + n : x.digits.end());

    BigInt y1(y.digits.size() > n ? y.digits.begin() + n : y.digits.end(), y.digits.end());
    BigInt y0(y.digits.begin(), y.digits.size() > n ? y.digits.begin() + n : y.digits.end());

    BigInt z0 = karatsuba(x0, y0);
    BigInt z1 = karatsuba(add(x0, x1), add(y0, y1));
    BigInt z2 = karatsuba(x1, y1);

    BigInt r1 = shift_left(z2, 2 * n);
    BigInt r2 = shift_left(subtract(subtract(z1, z2), z0), n);

    return add(add(r1, r2), z0);
}

// Экспоненцирование - Алгоритм бинарное возведение в степень
BigInt BigInt::pow(const BigInt& exponent) const {
    BigInt base = *this;
    BigInt exp = exponent;
    if (exp == zero) {
        return one;
    }
    else if (exp == one) {
        return base;
    }
    BigInt result("1");

    while (exp > zero) {
        if ((exp.digits[0] % 2) == 1) {
            result = karatsuba(result, base);
        }
        base = karatsuba(base, base);
        exp = exp / two;
    }

    return result;
}

BigInt BigInt::powmod(const BigInt& exponent, const BigInt& mod) const {
    BigInt base = *this;
    BigInt exp = exponent;
    if (exp == zero) {
        return one;
    }
    else if (exp == one) {
        return base;
    }
    BigInt result("1");

    while (exp > zero) {
        if ((exp.digits[0] % 2) == 1) {
            //result = karatsuba(result, base) % mod;
			result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp = exp / two;
    }

    return result;
}



BigInt BigInt::operator/(const BigInt& divisor) const {
    BigInt remainder;
    return divide(divisor, remainder);
}

std::string BigInt::toString() const {
    std::ostringstream oss;
    bool first = true;
    for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
        if (first) {
            oss << *it;
            first = false;
        }
        else {
            oss << std::setw(9) << std::setfill('0') << *it;
        }
    }
    return oss.str();
}


BigInt BigInt::mod(const BigInt& other) const {
    BigInt quotient;
    BigInt remainder;
    divide(other, remainder);
    return remainder;
}

BigInt BigInt::divide(const BigInt& divisor, BigInt& remainder) const {
    if (divisor == BigInt("0")) {
        throw std::invalid_argument("Division by zero");
    }

    BigInt dividend = *this;
    BigInt quotient;
    quotient.digits.resize(dividend.digits.size(), 0);

    BigInt current;
    for (int i = dividend.digits.size() - 1; i >= 0; --i) {
        current.digits.insert(current.digits.begin(), dividend.digits[i]);
        current.removeLeadingZeros();

        uint32_t x = 0, left = 0, right = 1000000000;
        while (left <= right) {
            uint32_t mid = (left + right) / 2;
            BigInt midVal = divisor.multiply(BigInt(std::to_string(mid)));
            if (midVal <= current) {
                x = mid;
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }

        quotient.digits[i] = x;
        current = current - divisor.multiply(BigInt(std::to_string(x)));
    }

    quotient.removeLeadingZeros();
    remainder = current;
    return quotient;
}
//BigInt BigInt::operator/(const BigInt& divisor) const {
//    if (divisor == zero) {
//        throw std::overflow_error("Divide by zero exception");
//    }
//
//    BigInt dividend = *this;
//    BigInt result("0");
//
//    int normalizationShift = 9 - std::to_string(divisor.digits.back()).length();
//    BigInt normalizedDivisor = divisor;
//    normalizedDivisor.digits.insert(normalizedDivisor.digits.begin(), normalizationShift, 0);
//
//    BigInt normalizedDividend = dividend;
//    normalizedDividend.digits.insert(normalizedDividend.digits.begin(), normalizationShift, 0);
//
//    for (int i = normalizedDividend.digits.size() - normalizedDivisor.digits.size(); i >= 0; --i) {
//        BigInt dividendSegment(
//            normalizedDividend.digits.begin() + i,
//            normalizedDividend.digits.begin() + i + normalizedDivisor.digits.size()
//        );
//
//        uint32_t quotientEstimate = dividendSegment.digits.back() / normalizedDivisor.digits.back();
//        BigInt quotientPart(std::to_string(quotientEstimate));
//
//        BigInt product = normalizedDivisor * quotientPart;
//        while (dividendSegment < product) {
//            quotientEstimate--;
//            quotientPart = BigInt(std::to_string(quotientEstimate));
//            product = karatsuba(normalizedDivisor, quotientPart);
//        }
//
//        dividendSegment = subtract(dividendSegment, product);
//        std::copy(dividendSegment.digits.begin(), dividendSegment.digits.end(), normalizedDividend.digits.begin() + i);
//
//        result.digits.push_back(quotientEstimate);
//    }
//
//    std::reverse(result.digits.begin(), result.digits.end());
//    result.removeLeadingZeros();
//
//    BigInt remainder = normalizedDividend; // Остаток после деления
//    remainder.removeLeadingZeros();
//
//    return result;
//}


