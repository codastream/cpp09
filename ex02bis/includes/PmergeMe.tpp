#include "PmergeMe.hpp"

template <typename C>
void PmergeMe::_sortRecursive(C& data, size_t elemSize)
{
	clock_t start;

	start = clock();
	merge(data, elemSize);
	_timeToMerge += clock() - start;

	start = clock();
	insert(data, elemSize);
	_timeToInsert += clock() - start;
}


template <typename C>
void PmergeMe::_insert(C& data, size_t elemSize)
{

}

template <typename T>
bool PmergeMe::_isFirstLowerMerge(T lv, T rv)
{
	PmergeMe::_nbCompMerge++;
	return *lv < *rv;
}

template <typename T>
bool PmergeMe::_isFirstLowerInsert(T lv, T rv)
{
	PmergeMe::_nbCompInsert++;
	return *lv < *rv;
}