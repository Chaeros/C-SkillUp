#ifndef SHARED_MEMORY_CONTRACT_H
#define SHARED_MEMORY_CONTRACT_H

#include <cstdint>

namespace sma {
    constexpr wchar_t kMappingName[] = L"Local\\AProjectBProjectSMA"; // sma 메모리 공간 이름으로 사용될 문자열

    struct SharedData {
        std::int32_t sequence;
        std::int32_t value;
    };

}

#endif