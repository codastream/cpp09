template <typename C>
void PmergeMe::_sortRecursive(C* data, size_t elemSize, int depth)
{
	clock_t start;
	size_t	nbElems;
	nbElems = data->size() / elemSize;

	#ifdef DEBUG
	printDepthOffset(depth);
	#endif

	if (nbElems < 2)
		return ;
	start = clock();
	_merge(data, elemSize, depth);
	timeToMerge += clock() - start;

	_sortRecursive(data, elemSize * 2, depth + 1);

	#ifdef DEBUG
	printDepthOffset(depth);
	#endif

	if (nbElems < 3)
		return ;

	start = clock();
	_insert(data, elemSize, depth);
	timeToInsert += clock() - start;
}

template <typename C>
void PmergeMe::sort(C* data)
{
	_sortRecursive(data, 1, 0);
}

template <typename T>
bool	PmergeMe::_isGreaterMerge(T lv, T rv)
{
	nbCompMerge++;
	return lv > rv;
}

template <typename T>
bool	PmergeMe::_isOngoingBissect(T start, T end)
{
	return start < end;
}

template <typename T>
bool	PmergeMe::_isLowerInsert(T found, T target)
{
	++nbCompInsert;
	if (DEB)
		std::cout << BG_WHITE << "+1 comparison : comparing if < " << found << " and " << target << NC << "... nbCompInsert now " << nbCompInsert << std::endl;
	return found < target;
}

template <typename T>
bool	PmergeMe::_isEqualInsert(T found, T target)
{
	++nbCompInsert;
	if (DEB)
		std::cout << BG_WHITE << "+1 comparison : comparing if == " << found << " and " << target << NC << "... nbCompInsert now " << nbCompInsert << std::endl;
	return found == target;
}

template <typename T>
bool	PmergeMe::_isUpperInsert(T lv, T rv)
{
	++nbCompInsert;
	if (DEB)
		std::cout << BG_WHITE << "+1 comparison : comparing if > " << lv << " and " << rv << NC << "... nbCompInsert now " << nbCompInsert << std::endl;
	return lv > rv;
}


template <typename T>
bool	PmergeMe::_isBeyondLast(T startIdx, T endIdx)
{
	if (DEB)
		std::cout << BG_WHITE << "+0 comparison : comparing if > " << startIdx << " and " << endIdx << NC << "... nbCompInsert now " << nbCompInsert << std::endl;
	return startIdx > endIdx;
}