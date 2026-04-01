#include "aThread.h"

#include <mutex>

#include "shared.h"

// 익명 네임스페이스로, aThread.cpp에서만 사용 가능하도록 제한함
namespace {
    int doNonCriticalWorkA(int i) {
        return (i * 17 + 3) % 11;
    }
}

void runAThread() {
    int localChecksum = 0;

    for (int i = 0; i < kIterationsPerThread; ++i) {
        localChecksum += doNonCriticalWorkA(i);

        std::lock_guard<std::mutex> lock(gCounterMutex);
        ++gCounter;
    }

    if (localChecksum == -1) {
        std::lock_guard<std::mutex> lock(gCounterMutex);
        gCounter += localChecksum;
    }
}