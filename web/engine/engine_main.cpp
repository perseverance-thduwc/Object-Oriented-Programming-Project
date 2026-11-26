// File: engine_main.cpp
// Tien trinh C++ doc lenh tu stdin (moi dong 1 lenh), in 1 dong JSON ra stdout.
// Duoc chay boi web/server.py.
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "GameEngine.h"

int main() {
    srand((unsigned int)time(0));
    GameEngine engine;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == "QUIT") break;
        std::cout << engine.handle(line) << std::endl;   // endl = flush
    }
    return 0;
}
