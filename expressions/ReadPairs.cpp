#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>
#include <string>

// Read a sequence of possibly whitespace-separated (name, value) pairs,
// where the name is a single word and the value is an integer or a
// floating-point value. Compute and print the sum and mean for each
// name and the sum and mean for all names.
int main()
{
	std::map<std::string, std::vector<float>> pairs;

	std::string name;
	float value;

	while (std::cin >> name >> value)
	{
		pairs[name].push_back(value);
	}

	float totalSum{ 0.0f };
	size_t totalCount{ 0 };

	for (auto& pair : pairs)
	{
		float nameSum{ 0.0f };

		std::cout << pair.first << "{";
		for (float& value : pair.second)
		{
			nameSum += value;
			std::cout << value << ", ";
		}
		std::cout << "\b\b}";

		float nameMean{ nameSum / pair.second.size() };
		std::cout << " sum{" << nameSum << "} mean{" << nameMean << "}\n";

		totalSum += nameSum;
		totalCount += pair.second.size();
	}

	float totalMean{ totalSum / totalCount };
	std::cout << "\nAll sum{" << totalSum << "} mean{" << totalMean << "}";

	return EXIT_SUCCESS;
}