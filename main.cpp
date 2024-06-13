#include <iostream>
#include <fstream>
#include "BigFloat.h" 

#include <ranges>
#include <algorithm>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <cstdlib>
#include <cstdint>
#include <filesystem>


template <std::invocable<BigInt, BigInt> F, std::ranges::range S>
void to_csv(std::ostream& os, F f, S sizes) {
    os << "Size,Time\r\n";
    for (auto cnt : sizes) {
        std::vector<std::uint32_t> left, right;
        std::generate_n(std::back_inserter(left), cnt, std::minstd_rand{ std::random_device{}() });
        std::generate_n(std::back_inserter(right), cnt, std::minstd_rand{ std::random_device{}() });

        std::transform(left.begin(), left.end(), left.begin(), [](uint32_t num) {
            return num % 1000000000; // Оставляем последние 9 цифр 
            });
        std::transform(right.begin(), right.end(), right.begin(), [](uint32_t num) {
            return num % 1000000000; // Оставляем последние 9 цифр
            });

        auto x = BigInt(std::begin(left), std::end(left)),
            y = BigInt(std::begin(right), std::end(right));
        auto t0 = std::chrono::steady_clock::now();
        auto z = f(x, y);
        auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t0).count();
        os << cnt << ',' << dt << "\r\n";
    }
}

void to_csv(std::filesystem::path file, auto f, std::size_t min_mmword_size, std::size_t max_mmword_size, std::size_t csv_row_count = 60) {
    auto os = std::ofstream(file, std::ios_base::out | std::ios_base::binary);
    if (!os.good())
        throw std::exception("Failed to open CSV");
    auto diff = max_mmword_size + 1 - min_mmword_size;
    auto sizes = std::views::iota(min_mmword_size * csv_row_count / diff, max_mmword_size * csv_row_count / diff)
        | std::views::transform([diff, csv_row_count](auto size) {return size * diff / csv_row_count << 4; });
    to_csv(os, f, sizes);
}



int main() {
    std::cout << "Addition..."; to_csv("add.csv", [](BigInt x, BigInt y) {return x + y; }, 1 << 12, 1 << 22); std::cout << "Done.\n";
    std::cout << "Subtraction..."; to_csv("sub.csv", [](BigInt x, BigInt y) {return x - y; }, 1 << 12, 1 << 22); std::cout << "Done.\n";
    std::cout << "Multuly..."; to_csv("mul.csv", [](BigInt x, BigInt y) {return x * y;}, 1 << 5, 1 << 12); std::cout << "Done.\n";
    std::cout << "Karatsuba..."; to_csv("kar.csv", karatsuba, 1 << 5, 1 << 12); std::cout << "Done.\n";
    std::cout << "Division..."; to_csv("div.csv", [](BigInt x, BigInt y) {return x / y; }, 1 << 8, 1 << 9); std::cout << "Done.\n";
    std::cout << "Pow..."; to_csv("pow.csv", [](BigInt x, BigInt y) {return x.pow(y);}, 1 << 2, 1 << 4); std::cout << "Done.\n";

#ifdef __clang__
    std::cout << "Compiled with Clang " << __clang_version__ << std::endl;
#elif defined(__GNUC__) || defined(__GNUG__)
    std::cout << "Compiled with GCC " << __VERSION__ << std::endl;
#elif defined(_MSC_VER)
    std::cout << "Compiled with MSVC " << _MSC_VER << std::endl;
#else
    std::cout << "Unknown compiler" << std::endl;
#endif
}
