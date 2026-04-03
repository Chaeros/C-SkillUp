#include "sma_reader.h"

#include <Windows.h>

#include <chrono>
#include <iostream>
#include <thread>

#include "shared_memory_contract.h"

int runSmaReaderExample() {
    std::cout << "[bProejct][SMA] reader 시작. aProject --sma-writer 실행 후 값을 읽습니다.\n";

    HANDLE mappingHandle = nullptr;
    for (int retry = 0; retry < 10; ++retry) {
        mappingHandle = OpenFileMappingW(FILE_MAP_READ, FALSE, sma::kMappingName); // 기존에 존재하는 sma::kMappingName 이름의 메모리 공간을 가져옴
        if (mappingHandle != nullptr) {
            break;
        }

        std::cout << "[bProejct][SMA] 매핑 대기 중...\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    if (mappingHandle == nullptr) {
        std::cerr << "[bProejct][SMA] OpenFileMapping 실패. Error=" << GetLastError() << '\n';
        return 1;
    }

    const void* view = MapViewOfFile(mappingHandle, FILE_MAP_READ, 0, 0, sizeof(sma::SharedData));
    if (view == nullptr) {
        std::cerr << "[bProejct][SMA] MapViewOfFile 실패. Error=" << GetLastError() << '\n';
        CloseHandle(mappingHandle);
        return 1;
    }

    const auto* shared = static_cast<const sma::SharedData*>(view);
    int lastSequence = -1;

    for (int i = 0; i < 12; ++i) {
        if (shared->sequence != lastSequence) {
            std::cout << "[bProejct][SMA] read  -> sequence=" << shared->sequence
                << ", value=" << shared->value << '\n';
            lastSequence = shared->sequence;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::cout << "[bProejct][SMA] reader 종료\n";

    UnmapViewOfFile(view);
    CloseHandle(mappingHandle);
    return 0;
}