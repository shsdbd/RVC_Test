// src/main.cpp
#include <iostream>
int main() {
    // 1. 명백한 메모리 누수 (Malloc/New 활용)
    int* leak = new int[1000];

    // 2. Clang-Tidy가 싫어하는 'if문 안에서 대입' (bugprone-assignment-in-if-condition)
    int a = 0;
    if (a = 10) {
        std::cout << "This should trigger an error!" << std::endl;
    }

    return 0;  // leak을 해제하지 않고 종료
}