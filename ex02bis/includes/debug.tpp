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
		if (isOdd(i, elemSize, dataSize, nbElems))
			std::cout << WHITE << std::right << std::setw(width - 1) << *it << " " << NC;
		else if (isLastNbOfPair(i, elemSize))
			std::cout << maxCol << std::right << std::setw(width - 1) << *it << GREY << "]" << YELLOW;
		else if (isFirstNbOfPair(i, elemSize))
			std::cout << GREY << "[" << YELLOW << std::right << std::setw(width - 2) << *it << " " << NC;
		else if (isLastNbOfElem(i, elemSize) && !isLastNbOfOdd(i, elemSize, dataSize, nbElems))
			std::cout << YELLOW << BOLD_ON << std::right << std::setw(width - 1) << *it << BOLD_OFF << GREY << "," << YELLOW;
		else
			std::cout << std::right << std::setw(width - 1) << *it << " ";
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