template <typename C>
bool	isSortedAsc(const C& cont, size_t elemSize)
{
	if (cont.size() < elemSize * 2)
		return true;
	typename C::const_iterator previt = cont.begin();
	std::advance(previt, elemSize - 1);
	typename C::const_iterator it = previt;
	std::advance(it, elemSize);
	for (;;)
	{
		if (*it < *previt)
		{
			std::cout << *it << " comes after " << *previt << std::endl;
			return false;
		}
		typename C::const_iterator probe = it;
		size_t k = elemSize;
		while (k-- && probe != cont.end())
			++probe;
		if (probe == cont.end())
			break ;
		std::advance(previt, elemSize);
		std::advance(it, elemSize);
	}
	return true;
}

template <typename C>
bool	hasAllElemsOfFirst(const C& first, const C& second)
{
	for (typename C::const_iterator it = first.begin(); it != first.end(); ++it)
	{
		if (std::find(second.begin(), second.end(), *it) == second.end())
		{
			return false;
		}
	}
	return true;
}

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
			std::cout << BLUE << std::right << std::setw(width) << *it << GREY << ' ' << NC;
		else
			std::cout << YELLOW << std::right << std::setw(width) << *it << GREY << ' ';
		++it;
	}
	std::cout << NC << std::endl;
}

template <typename C>
void	printMain(const std::string& msgCol, const C* pending, int depth, size_t elemSize, bool showMax)
{
	(void) depth;

	std::cout << msgCol << YELLOW;
	int	width = 7;
	size_t size = pending->size();
	typename C::const_iterator it = pending->begin();
	for (size_t i = 0; i < size; ++i, ++it)
	{
		if (showMax && ((i + 1) % elemSize) == 0)
			std::cout << GREEN << std::right << std::setw(width) << *it << GREY << "," << NC;
		else
			std::cout << YELLOW << std::right << std::setw(width) << *it << GREY << " " << NC;
	}
	std::cout << NC << std::endl;
}

template <typename C>
void	printPending(const std::string& msgCol, const C* pending, int depth, size_t elemSize, int insertedNb, bool showMax)
{
	(void) depth;
	int	width = 7;

	std::cout << msgCol << YELLOW;

	if (insertedNb > 0)
	{
		for (size_t i = 0; i < elemSize; ++i)
			std::cout << "                ";
	}
	size_t dataSize = pending->size();
	typename C::const_iterator it = pending->begin();
	for (size_t i = 0; i < dataSize; ++i, ++it)
	{
		if (showMax && i != 0 && (i % elemSize) - 1 == 0)
		{
			std::cout << YELLOW << BOLD_ON << std::right << std::setw(width) << std::setfill(' ') << *it << GREY << "," << std::setfill(' ') << NC;
		}
		else
		{
			if (i > 0 && i % elemSize * 2 == 0)
			{
				for (size_t i = 0; i < elemSize; ++i)
					std::cout << "       ";
			}
			std::cout << YELLOW << std::right << std::setw(width) << std::setfill(' ') << *it << NC << " " << std::setfill(' ');
		}
	}
	std::cout << NC << std::endl;
}