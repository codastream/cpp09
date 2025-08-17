#include "PmergeMe.hpp"

/***************************************************************
*					📁 CLASS ATTRIBUTES							*
***************************************************************/

const int PmergeMe::_jacobLen = 21;

/***************************************************************
*					📁 CLASS FUNCTIONS							*
***************************************************************/

std::vector<int>	PmergeMe::_generateJSequence()
{
	std::vector<int> v;
	v.push_back(0);
	v.push_back(1);
	for(int i = 2; i < PmergeMe::_jacobLen; ++i)
	{
		v.push_back(v[i - 1] + 2 * v[i - 2]);
	}
	return v;
}

/***************************************************************
*					🥚 CTORS / DTOR								*
***************************************************************/

PmergeMe::PmergeMe() : 
	timeToMerge(0), timeToInsert(0), nbCompMerge(0), nbCompInsert(0), vec(), _jacob(PmergeMe::_generateJSequence()) {}

PmergeMe::PmergeMe(const PmergeMe& other) : 
	timeToMerge(0), timeToInsert(0), nbCompMerge(0), nbCompInsert(0), vec(other.vec), _jacob(other._jacob) {}

PmergeMe::~PmergeMe() {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
	if (this != &other)
		vec = other.vec;
	return (*this);
}

/***************************************************************
*					🛠️ STATIC FUNCTIONS						*
***************************************************************/

static void swapPairs(t_vec::iterator itMax, t_vec::iterator itMin, size_t elemSize)
{
	for (size_t i = 0; i < elemSize; i++)
		std::swap(*itMax--, *itMin--);
}

/// @brief generate sorted container main (aka S) made of first elem of mins (aka B) and all elems of maxs (aka A)
/// @param main 
/// @param data 
/// @param elemSize 
static void	fillMain(t_vec& main, t_vec& data, size_t elemSize)
{
	size_t	dataSize = data.size();
	size_t	mainSize = dataSize / elemSize;

	main.push_back(data[elemSize - 1]);
	for (size_t i = 1; i < mainSize; i += elemSize)
	{
		main.push_back(data[i]);
	}
}

/***************************************************************
*					📁 MEMBER FUNCTIONS						*
***************************************************************/

void	PmergeMe::initVec(int ac, char **av)
{
	int		val;
	char	*end;

	for (int i = 0; i < ac; i++)
	{
		val = static_cast<int>(std::strtol(av[i], &end, 10));
		vec.push_back(val);
	}
}

size_t PmergeMe::_computeBatchSize(size_t n)
{
	if (n == 0)
		return 0;
	else if (n >= _jacobLen)
		throw std::out_of_range("trying to reach beyond registered elems of Jacobstahl sequence");
	return (_jacob[n] - _jacob[n - 1]);
}

t_vec::iterator	PmergeMe::_binarySearchIt(t_vec& main, size_t start, size_t end, int val)
{
	size_t	insertIdx = 0;
	while (_isOngoingBissect(end, start))
	{
		int mid = start + (end - start) / 2;
		if (_isGreaterMerge(main[mid], val))
		{
			end = mid - 1;
			insertIdx = mid;
		}
		else
		{
			start = mid + 1;
			insertIdx = mid + 1;
		}
	}
	return main.begin() + insertIdx;
}

/// @brief creates pairs (min, max)
/// @param data 
/// @param elemSize 
void	PmergeMe::_merge(t_vec& data, size_t elemSize)
{
	#ifdef DEBUG
	printByPair(BLUE "before merge:\t", data, 0, elemSize);
	#endif
	size_t	offsetFirst = elemSize - 1;
	size_t	pairSize = elemSize * 2;
	size_t	offsetSecond = pairSize - 1;
	t_vec::iterator itFirst = data.begin() + offsetFirst;
	t_vec::iterator itSecond = data.begin() + offsetSecond;
	for (;
		itFirst < data.end() - 1;
		itFirst += pairSize, itSecond += pairSize
	)
	{
		if (_isGreaterMerge(itFirst, itSecond))
			swapPairs(itFirst, itSecond, elemSize);
	}
	#ifdef DEBUG
	printByPair(BLUE "after merge:\t", data, 0, elemSize);
	#endif
}

/// @brief binary inserts pending + odd elems into main, first by bounding with Jacobstahl sequence, then for all the surface of main
/// @param data 
/// @param elemSize 
void	PmergeMe::_insert(t_vec& data, size_t elemSize)
{
	t_vec	main;
	fillMain(main, data, elemSize);
	#ifdef DEBUG
	printData(BLUE "main:\t\t", main, 0, elemSize);
	#endif

	size_t					k = 3;
	size_t					tk = _jacob[k];
	size_t					batchSize = tk - _jacob[k - 1];
	size_t					pendingNb = data.size() % 2 == 0 ? data.size() / 2 : data.size() / 2 + 1;
	size_t					i;
	int						toInsert;
	t_vec::iterator			insertPos;
	
	while (batchSize <= pendingNb)
	{
		i = k;
		while (batchSize > 0 && i < data.size())
		{
			toInsert = data[i];
			insertPos = _binarySearchIt(main, 0, tk + 1, toInsert);
			main.insert(insertPos, toInsert);
			--pendingNb;
			--i;
			--batchSize;
		}
		++k;
		if (k > _jacobLen)
			throw std::out_of_range("trying to reach beyond registered elems of Jacobstahl sequence");
		tk = _jacob[k];
		batchSize = tk - _jacob[k - 1];
	}
	while (pendingNb > 0)
	{
		toInsert = data[k];
		insertPos = _binarySearchIt(main, 0, main.size(), toInsert);
		--pendingNb;
		++k;
	}
	#ifdef DEBUG
	printData(BLUE "sorted:\t\t", main, 0, elemSize);
	#endif
}
