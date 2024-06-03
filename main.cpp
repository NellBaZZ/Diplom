#include <iostream>
#include "BigFloat.h" 

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    BigInt a("512");//14002965526155144834555305452685214372866
    BigInt b("512");
    BigInt c("3");
    BigInt result = a^b;
    result = result/c;
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

//int main() {
//    BigFloat num2("123.656789123456789123456789123456789123456789123456789");//123.656789123456789123456789123456789123456789123456789
//    BigFloat num1("987.96543219");//987.9654321987654321987654321987654321987654321987654321
//
//     BigFloat resultAdd = num1 + num2;
//     BigFloat resultSub = num1 - num2;
//    BigFloat resultMul = num1 * num2;
//    BigFloat resultDiv = num1 / num2;
//    BigInt a("987");//14002965526155144834555305452685214372866
//    BigInt b("123");
//    BigInt result = a / b;
//    std::cout << "qwe" << std::endl;
//       result.print();
//     
//    std::cout << "Addition: ";
//     resultAdd.print();
//    std::cout << "Subtraction: ";
//     resultSub.print();
//    std::cout << "Multiplication: ";
//    resultMul.print();
//    std::cout << "Division: ";
//    resultDiv.print();
//
//    return 0;
//}