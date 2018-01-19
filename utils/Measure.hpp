#ifndef _MEASURE_HPP_
#define _MEASURE_HPP_

#include <chrono>
#include <functional>


long long measure(std::function<void(void)> f)
{
	auto start = std::chrono::high_resolution_clock::now();
	f();
	auto end = std::chrono::high_resolution_clock::now();
	return (end - start).count();
}


#endif // _MEASEURE_HPP_
