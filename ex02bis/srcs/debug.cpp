#include "debug.hpp"

void printDepthOffset(int depth)
{
	for (int i = 0; i < depth; ++i)
		std::cout << " ";
}

bool	isLastNbOfElem(size_t i, size_t elemSize)
{
	return (i + 1) % elemSize == 0;
}

bool	isFirstNbOfPair(size_t i, size_t elemSize)
{
	return (i + elemSize * 2) % (elemSize * 2) == 0;
}

bool	isLastNbOfSecondPair(size_t i, size_t elemSize)
{
	return (i + 1) % (elemSize * 2) == 0;
}

bool	isOdd(size_t i, size_t elemSize, size_t dataSize, size_t nbElems)
{
	return (nbElems % 2 == 1 && i >=  dataSize - elemSize);
}

bool	isLastNbOfOdd(size_t i, size_t elemSize, size_t dataSize, size_t nbElems)
{
	return (nbElems % 2 == 1 && isLastNbOfElem(i, elemSize) && i == dataSize - 1);
}