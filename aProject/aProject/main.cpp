#include <chrono>
#include <iostream>
#include <thread>

#include "aThread.h"
#include "bThread.h"
#include "shared.h"

int main() {
    auto start = std::chrono::steady_clock::now();

    std::thread aThread(runAThread);
    std::thread bThread(runBThread);

    aThread.join();
    bThread.join();

    auto end = std::chrono::steady_clock::now();
    const auto elapsedMs =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    const int expected = kIterationsPerThread * 2;
    const bool raceConditionDetected = (gCounter != expected);

    std::cout << "Final counter: " << gCounter << '\n';
    std::cout << "Expected     : " << expected << '\n';
    std::cout << "Elapsed(ms)  : " << elapsedMs << '\n';

    // Mutex가 제대로 적용되지 않아 Race Condition이 발생한 경우
    if (raceConditionDetected) {
        std::cout << "[FAIL] Race condition detected." << '\n';
        return 1;
    }

    // Mutex가 제대로 적용되어 Race Condition이 발생하지 않은 경우
    std::cout << "[PASS] No race condition: extern mutex shared between split threads." << '\n';
    return 0;
}