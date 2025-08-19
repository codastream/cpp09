#include "debug.hpp"

void printDepthOffset(int depth)
{
	std::cout << GREY << std::right << std::setfill('-') << std::setw(90) << "depth " << std::setfill(' ') << depth  << " / elemSize " << std::pow(2, depth) << NC << std::endl;
}

bool	isLastNbOfElem(size_t i, size_t elemSize)
{
	return (i + 1) % elemSize == 0;
}

bool	isFirstNbOfPair(size_t i, size_t elemSize)
{
	return (i + elemSize * 2) % (elemSize * 2) == 0;
}

bool	isLastNbOfPair(size_t i, size_t elemSize)
{
	return (i + 1) % (elemSize * 2) == 0;
}

bool	isOdd(size_t i, size_t elemSize, size_t dataSize)
{
	size_t pairSize = elemSize * 2;
	return (dataSize / pairSize % 2 != 0 && i >  dataSize - pairSize);
}

bool	isLastNbOfOdd(size_t i, size_t elemSize, size_t dataSize, size_t nbElems)
{
	return (nbElems % 2 == 1 && isLastNbOfElem(i, elemSize) && i == dataSize - 1);
}