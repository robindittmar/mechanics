#include "Benchmark.h"

CBenchmark::CBenchmark(bool bStart)
{
	if (bStart)
		this->StartBenchmark();
}

CBenchmark::~CBenchmark()
{
}

void CBenchmark::StartBenchmark()
{
	m_timeStart = high_resolution_clock::now();
}

void CBenchmark::FinishBenchmark()
{
	m_timeEnd = high_resolution_clock::now();
}

long long CBenchmark::GetElapsedTime()
{
	return (duration_cast<microseconds>(m_timeEnd - m_timeStart)).count();
}

void CBenchmark::PrintBenchmark(const char* pName)
{
#ifdef _DEBUG
	g_pConsole->Write(LOGLEVEL_INFO, "Benchmark [%s] finished after %ld microseconds\n", pName, this->GetElapsedTime());
#endif // _DEBUG
}