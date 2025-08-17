template <typename C>
void PmergeMe::_sortRecursive(C& data, size_t elemSize)
{
	clock_t start;
	size_t	nbElems;

	nbElems = data.size() / elemSize;
	if (nbElems < 2)
		return ;
	start = clock();
	_merge(data, elemSize);
	timeToMerge += clock() - start;

	_sortRecursive(data, elemSize * 2);

	if (nbElems < 3)
		return ;

	start = clock();
	_insert(data, elemSize);
	timeToInsert += clock() - start;
}

template <typename C>
void PmergeMe::sort(C& data)
{
	_sortRecursive(data, 1);
}

template <typename T>
bool PmergeMe::_isGreaterMerge(T lv, T rv)
{
	nbCompMerge++;
	return lv > rv;
}

template <typename T>
bool PmergeMe::_isOngoingBissect(T start, T end)
{
	nbCompMerge++;
	return start <= end;
}

template <typename T>
bool PmergeMe::_isGreaterInsert(T lv, T rv)
{
	nbCompInsert++;
	return *lv > *rv;
}