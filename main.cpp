#include <iostream>
#include "BigInt.h" 

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    BigInt a("512");
    BigInt b("512");
    BigInt result = a^b;
    result.print();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken by function: " << duration.count() << " seconds" << std::endl;

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