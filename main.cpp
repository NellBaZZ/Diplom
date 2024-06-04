#include <iostream>
#include "BigFloat.h" 

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    BigInt qwe("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
    BigInt asd("12345678909654321987654321987654321987654321987654321987654321");
    BigInt a("512");//14002965526155144834555305452685214372866
    BigInt b("512");
    BigInt c("3");
    
    BigInt resultAddInt = qwe + asd;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken by function add:" << duration.count() << " seconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    BigInt resultSubInt = qwe - asd;
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Time taken by function substruct" << duration.count() << " seconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    BigInt resultMulInt = qwe * asd;
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Time taken by function exp: " << duration.count() << " seconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    BigInt resultDivInt = a / b;
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Time taken by function div: " << duration.count() << " seconds" << std::endl;


    std::cout << "Addition Int: ";
    resultAddInt.print();
    std::cout << "Subtraction Int: ";
    resultSubInt.print();
    std::cout << "Multiplication Int: ";
    resultMulInt.print();
    std::cout << "Division Int: ";
    resultDivInt.print();

    std::cout << std::endl;

    BigFloat num1("1234567890.656789123456789123456789123456789123456789123456789");//123.656789123456789123456789123456789123456789123456789
    BigFloat num2("111111.9654321987654321987654321987654321987654321987654321");//987.9654321987654321987654321987654321987654321987654321

    BigFloat resultAdd = num1 + num2;
    BigFloat resultSub = num1 - num2;
    BigFloat resultMul = num1 * num2;
    BigFloat resultDiv = num1 / num2;
     
    std::cout << "Addition Float: ";
     resultAdd.print();
    std::cout << "Subtraction FLoat: ";
     resultSub.print();
    std::cout << "Multiplication Float: ";
    resultMul.print();
    std::cout << "Division Float: ";
    resultDiv.print();



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