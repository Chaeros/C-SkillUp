#ifndef APROJECT_SHARED_HPP
#define APROJECT_SHARED_HPP

#include <mutex>

// 다른 파일에서 전역 공유 객체로 쓰기 위해 extern 처리
// 실제 객체는 shared.cpp 내부에 있다.
// 다른 파일에서 사용하기 위해서는 shared.h include 해야함
extern int gCounter;
extern std::mutex gCounterMutex;

constexpr int kIterationsPerThread = 1'000'000; // 상수 표현식으로 컴파일 타임에 값으로 사용

#endif