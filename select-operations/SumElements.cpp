#include <iostream>
#include <cstdlib>
#include <vector>
#include <numeric>

#include "Measure.hpp"


template<class Type>
void test(const Type max);


/// Sum elements with a normal for
template<class Iterator, class T>
inline T sum(Iterator first, const Iterator last, T result)
{
	for(; first != last; ++first)
	{
		result += *first;
	}
	return result;
}

/// Write a simple loop calculating a sum of elements.
/// See if you can find any performance differences between the different versions
int main(int argc, char** argv)
{
	char type{ argv[1][0] };
	int max{ atoi(argv[2]) };
	switch(type)
	{
		case 'i':
			std::cout << "Integers\n";
			test(static_cast<int>(max));
			break;
		case 'f':
			std::cout << "Float\n";
			test(static_cast<float>(max));
			break;
		default:
			std::cerr << "Error\n";
			return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


template<class Type>
void test(const Type max)
{
	// Populate a vector with a lot of integers
	std::vector<Type> container;
	for(Type i{ 0 }; i < max; ++i)
	{
		container.push_back(i);
	}

	Type init = 0LL;

	// Sum using accumulate
	auto callAccumulate = [&container, &init](){
		Type result{ std::accumulate(container.begin(), container.end(), init) };
		std::cout << "Accumulate = " << result;
	};

	// Sum using a normal for
	auto callSum = [&container, &init](){
		Type result{ sum(container.begin(), container.end(), init) };
		std::cout << "Sum = " << result;
	};

	std::cout << " (" << measure(callAccumulate) / 1000000 << " milliseconds)\n";
	std::cout << " (" << measure(callSum) / 1000000 << " milliseconds)\n";
}
