#include "debug.hpp"

template <typename C>
void	printByPair(const std::string& msgCol, const C& data, int depth, size_t elemSize)
{
	printDepthOffset(depth);
	std::cout << msgCol << YELLOW;

	size_t dataSize = data.size();
	typename T::const_iterator it = data.begin();
	for (size_t i = 0; i < dataSize; ++i)
	{
		if (i % elemSize == 0 && i != dataSize - 1)
				std::cout << '[' << std::right << std::setw(4) << *it;
		else if (i + 1 % elemSize == 0)
			std::cout << BOLD_ON << std::right << std::setw(4) << *it << BOLD_OFF << ']';
		else
			std::cout << std::right << std::setw(5) << *it;
		
		if (i != dataSize - 1)
			std::cout << ' ';
		++it;
	}
	std::cout << NC << std::endl;
}

template <typename C>
void	printData(const std::string& msgCol, const C& data, int depth, size_t elemSize)
{
	printDepthOffset(depth);
	std::cout << msgCol << YELLOW;

	size_t dataSize = data.size();
	typename T::const_iterator it = data.begin();
	for (size_t i = 0; i < dataSize; ++i)
	{
		std::cout << std::right << std::setw(5) << *it << ' ';
		++it;
	}
	std::cout << NC << std::endl;
}