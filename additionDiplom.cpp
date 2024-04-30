#include <immintrin.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <bit> 


void addLongNumbersAVX512(const std::vector<uint64_t>& a, const std::vector<uint64_t>& b, std::vector<uint64_t>& result) {
    size_t maxLen = std::max(a.size(), b.size());
    result.resize(maxLen);
    // Cи вектора и размеры  
    bool carry = false; 

    size_t i = 0;
    for (; i + 8 <= maxLen; i += 8) {
        __m512i vecA = _mm512_loadu_si512((__m512i*)(i < a.size() ? &a[i] : &result[i])); //использование result для недостающих элементов
        __m512i vecB = _mm512_loadu_si512((__m512i*)(i < b.size() ? &b[i] : &result[i])); 
        __m512i sum = _mm512_add_epi64(vecA, vecB);

        if (carry) {
            __m512i vecCarry = _mm512_set1_epi64(1);
            sum = _mm512_add_epi64(sum, vecCarry);

            // остался ли перенос
            __mmask8 mask = _mm512_test_epi64_mask(vecCarry, vecCarry);
            carry = (mask != 0);
        }

        _mm512_storeu_si512((__m512i*)&result[i], sum);

        // Проверка переноса для текущего блока
        __mmask8 mask = _mm512_test_epi64_mask(sum, _mm512_set1_epi64(0x8000000000000000));
        carry = (mask != 0);
    }

    // Обработка оставшихся элементов, если размер не кратен 8
    for (; i < maxLen; ++i) {
        uint64_t sum = (i < a.size() ? a[i] : 0) + (i < b.size() ? b[i] : 0) + (carry ? 1 : 0);
        carry = sum < a[i] || (carry && sum <= a[i]);
        result[i] = sum;
    }

    // Если после всего сложения остался перенос, добавляем еще один элемент в результат
    if (carry) {
        result.push_back(1);
    }
}

int main() {

    std::vector<uint64_t> num1 = {
        0x123456789ABCDEF0, 0xFFFFFFFF00000000, 0x123456789ABCDEF0
    };

    std::vector<uint64_t> num2 = {
        0xFEDCBA9876543210, 0x00000000FFFFFFFF, 0xFEDCBA9876543210
    };
    //std::vector<uint64_t> num1 = {
    //   1000000, 0, 0
    //};

    //std::vector<uint64_t> num2 = {
    //    2333322, 0, 0
    //};

    std::vector<uint64_t> result;

    addLongNumbersAVX512(num1, num2, result);

    std::cout << "Result: ";
    for (auto it = result.rbegin(); it != result.rend(); ++it) {
        std::cout << std::hex << *it << "";
    }
    std::cout << std::dec << "\n";

    return 0;
}