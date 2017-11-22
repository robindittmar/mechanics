#ifndef __BENCHMARK_H__
#define __BENCHMARK_H__

// Std Lib
#include <chrono>

// Source SDK

// Custom
#include "Console.h"

using namespace std::chrono;

class CBenchmark
{
public:
	CBenchmark(bool bStart = false);
	~CBenchmark();

	void StartBenchmark();
	void FinishBenchmark();

	long long GetElapsedTime();

	void PrintBenchmark(const char* pName);
private:
	high_resolution_clock::time_point m_timeStart;
	high_resolution_clock::time_point m_timeEnd;
};

#endif // __BENCHMARK_H__