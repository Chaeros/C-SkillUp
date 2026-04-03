#include "sma_writer.h"

#include <Windows.h>

#include <chrono>
#include <iostream>
#include <thread>

#include "shared_memory_contract.h"

int runSmaWriterExample() {
    // sma::kMappingName 이 이름을 기반으로 메모리에 공유하는 sma 공간 확보, 명칭 동일하면 같은 메모리 공간 공유함
    HANDLE mappingHandle = CreateFileMappingW(
        INVALID_HANDLE_VALUE,
        nullptr,
        PAGE_READWRITE,
        0,
        sizeof(sma::SharedData),
        sma::kMappingName);

    if (mappingHandle == nullptr) {
        std::cerr << "[aProject][SMA] CreateFileMapping 실패. Error=" << GetLastError() << '\n';
        return 1;
    }

    // MapViewOfFile : OS가 만든 공유 메모리를 내 프로세스 주소 공간에 매핑해서 포인터로 쓰게 해주는 함수
    // view는 메모리 주소를 가져온다. 근데 타입없는 상태
    void* view = MapViewOfFile(mappingHandle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(sma::SharedData));
    if (view == nullptr) {
        std::cerr << "[aProject][SMA] MapViewOfFile 실패. Error=" << GetLastError() << '\n';
        CloseHandle(mappingHandle);
        return 1;
    }

    // 타입이 지정안된 메모리에 캐스팅을 통해 타입 지정, 이를 통해 이후 해당 메모리에 접근 가능
    // auto를 통해 컴파일러가 타입을 자동 추론, const sma::SharedData* 로 추론할 것임. const-> 읽기 전용
    auto* shared = static_cast<sma::SharedData*>(view);
    shared->sequence = 0;
    shared->value = 100;

    std::cout << "[aProject][SMA] writer 시작. bProejct reader를 실행하세요.\n";

    for (int i = 1; i <= 10; ++i) {
        shared->sequence = i;
        shared->value += 5;

        std::cout << "[aProject][SMA] write -> sequence=" << shared->sequence
            << ", value=" << shared->value << '\n';

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "[aProject][SMA] writer 종료\n";

    UnmapViewOfFile(view);      // 메모리 연결 해제
    CloseHandle(mappingHandle); // 공유 메모리 객체 정리
    return 0;
}