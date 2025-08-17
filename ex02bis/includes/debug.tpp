template <typename C>
void	printByPair(const std::string& msgCol, const C& data, int depth, size_t elemSize)
{
	printDepthOffset(depth);
	std::cout << msgCol << YELLOW;

	size_t dataSize = data.size();
	size_t nbElems = dataSize / elemSize;
	typename C::const_iterator it = data.begin();
	for (size_t i = 0; i < dataSize; ++i, ++it)
	{
		if (isOdd(i, elemSize, dataSize, nbElems))
			std::cout << WHITE << std::right << std::setw(5) << *it << ' ' << NC;
		else if (isLastNbOfSecondPair(i, elemSize))
			std::cout << CYAN << BOLD_ON << std::right << std::setw(5) << *it << YELLOW << "]";
		else if (isFirstNbOfPair(i, elemSize))
			std::cout << '[' << std::right << std::setw(4) << *it << ' ';
		else if (isLastNbOfElem(i, elemSize) && !isLastNbOfOdd(i, elemSize, dataSize, nbElems))
			std::cout << BOLD_ON << std::right << std::setw(5) << *it << BOLD_OFF << ",";
		else
			std::cout << std::right << std::setw(5) << *it << ' ';
	}
	std::cout << NC << std::endl;
}

template <typename C>
void	printData(const std::string& msgCol, const C& data, int depth, size_t elemSize, bool showMax)
{
	printDepthOffset(depth);
	std::cout << msgCol << YELLOW;

	size_t dataSize = data.size();
	typename C::const_iterator it = data.begin();
	for (size_t i = 0; i < dataSize; ++i)
	{
		if (showMax && i == elemSize * 2 - 1)
			std::cout << BLUE << std::right << std::setw(5) << *it << ' ' << NC;
		else
			std::cout << std::right << std::setw(5) << *it << ' ';
		++it;
	}
	std::cout << NC << std::endl;
}