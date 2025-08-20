template <typename C>
void	printByPair(const std::string& msgCol, const C* data, int depth, size_t elemSize, bool showMax)
{
	(void) depth;

	std::cout << msgCol << YELLOW;
	const char* maxCol = showMax ? CYAN : YELLOW;
	int	width = 8;
	size_t dataSize = data->size();
	size_t nbElems = dataSize / elemSize;
	typename C::const_iterator it = data->begin();
	for (size_t i = 0; i < dataSize; ++i, ++it)
	{
		if (isOdd(i, elemSize, dataSize))
			std::cout << WHITE << std::right << std::setw(width - 1) << *it << " " << NC;
		else if (isLastNbOfPair(i, elemSize))
			std::cout << YELLOW << maxCol << std::right << std::setw(width - 1) << *it << GREY << "]" << YELLOW;
		else if (isFirstNbOfPair(i, elemSize))
			std::cout << GREY << "[" << YELLOW << std::right << std::setw(width - 2) << *it << " " << NC;
		else if (isLastNbOfElem(i, elemSize) && !isLastNbOfOdd(i, elemSize, dataSize, nbElems))
			std::cout << YELLOW << std::right << std::setw(width - 1) << *it << GREY << "," << YELLOW;
		else
			std::cout << YELLOW << std::right << std::setw(width - 1) << *it << " ";
	}
	std::cout << NC << std::endl;
}

template <typename C>
void	printByPairWithPending(const std::string& msgCol, const C* data, int depth, size_t elemSize, size_t batchEndIdx, size_t batchSize, bool showMax)
{
	(void) depth;

	std::cout << msgCol << YELLOW;
	const char* maxCol = showMax ? CYAN : YELLOW;
	const char* batchColor = PURPLE;
	int	width = 8;
	size_t dataSize = data->size();
	size_t nbElems = dataSize / elemSize;
	typename C::const_iterator it = data->begin();
	for (size_t i = 0; i < dataSize; ++i, ++it)
	{
		if (isOdd(i, elemSize, dataSize))
			std::cout << WHITE << std::right << std::setw(width - 1) << *it << " " << NC;
		else if (isLastNbOfPair(i, elemSize))
			std::cout << YELLOW << maxCol << std::right << std::setw(width - 1) << *it << GREY << "]" << YELLOW;
		else if (isFirstNbOfPair(i, elemSize) && isPartOfBatch(i, elemSize, batchEndIdx, batchSize))
			std::cout << GREY << "[" << batchColor << std::right << std::setw(width - 2) << *it << " " << NC;
		else if (isFirstNbOfPair(i, elemSize))
			std::cout << GREY << "[" << YELLOW << std::right << std::setw(width - 2) << *it << " " << NC;
		else if (isLastNbOfElem(i, elemSize) && !isLastNbOfOdd(i, elemSize, dataSize, nbElems) && isPartOfBatch(i, elemSize, batchEndIdx, batchSize))
			std::cout << batchColor << std::right << std::setw(width - 1) << *it << GREY << "," << YELLOW;
		else if (isLastNbOfElem(i, elemSize) && !isLastNbOfOdd(i, elemSize, dataSize, nbElems))
			std::cout << YELLOW << std::right << std::setw(width - 1) << *it << GREY << "," << YELLOW;
		else if (isPartOfBatch(i, elemSize, batchEndIdx, batchSize))
			std::cout << batchColor << std::right << std::setw(width - 1) << *it << " ";
		else
			std::cout << YELLOW << std::right << std::setw(width - 1) << *it << " ";
	}
	std::cout << NC << std::endl;
}

template <typename C>
void	printData(const std::string& msgCol, const C* data, int depth, size_t elemSize, bool showMax)
{
	(void) depth;

	std::cout << msgCol << YELLOW;
	int	width = 7;
	size_t dataSize = data->size();
	typename C::const_iterator it = data->begin();
	for (size_t i = 0; i < dataSize; ++i)
	{
		if (showMax && i == elemSize * 2 - 1)
			std::cout << BLUE << std::right << std::setw(width) << *it << ' ' << NC;
		else
			std::cout << std::right << std::setw(width) << *it << ' ';
		++it;
	}
	std::cout << NC << std::endl;
}