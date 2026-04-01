#include "bThread.h"

#include <mutex>

#include "shared.h"

namespace {
    int doNonCriticalWorkB(int i) {
        return (i * 29 + 7) % 13;
    }
}

void runBThread() {
    int localChecksum = 0;

    for (int i = 0; i < kIterationsPerThread; ++i) {
        localChecksum += doNonCriticalWorkB(i);

        std::lock_guard<std::mutex> lock(gCounterMutex);
        ++gCounter;
    }

    if (localChecksum == -1) {
        std::lock_guard<std::mutex> lock(gCounterMutex);
        gCounter += localChecksum;
    }
}