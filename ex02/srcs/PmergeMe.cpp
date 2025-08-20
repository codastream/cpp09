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

static size_t getIndexOfLastNb(size_t elemIndex, size_t elemSize)
{
	size_t res = (elemIndex + 1) * elemSize - 1;
	return res;
}

static void	insertElemAtBack(t_vec& main, t_vec& data, size_t elemSize, size_t fromIndex)
{
	for (size_t i = 0; i < elemSize && data[fromIndex]; ++i, ++fromIndex)
	{
		if (DEB)
		{
			std::cout << "adding data[" << fromIndex << "]= " << data[fromIndex] << " to end of main\n";
		}
		main.push_back(data[fromIndex]);
	}
}

static void insertElemAtPos(t_vec& main, t_vec* data, size_t elemSize, size_t fromIndex, size_t insertOffset)
{
	for (size_t i = 0; i < elemSize && fromIndex < data->size(); ++i, ++fromIndex)
	{
		main.insert(main.begin() + insertOffset + i, (*data)[fromIndex]);
	}
}

/// @brief generate sorted container main (aka S) made of first elem of mins (aka B) and all elems of maxs (aka A)
/// @param main 
/// @param data 
/// @param elemSize 
static void	fillMain(t_vec& main, t_vec* data, size_t elemSize, size_t nbElem)
{
	(void) nbElem;
	size_t	dataSize = data->size();
	size_t	pairSize = elemSize * 2;
	
	#ifdef DEBUG
	std::cout << PURPLE << "\nFilling main....\n" << NC;
	std::cout << PURPLE << "Adding B1 (min elem of first sorted pair)" << NC << std::endl;
	#endif
	insertElemAtBack(main, *data, elemSize, 0);
	#ifdef DEBUG
	printData(BLUE "main:\t", &main, 0, elemSize);
	std::cout << PURPLE << "Adding A1..n (max elems of sorted pair)" << NC << std::endl;
	#endif
	for (size_t i = elemSize; i <= dataSize - elemSize; i += pairSize)
	{
		insertElemAtBack(main, *data, elemSize, i);
	}
	#ifdef DEBUG
	printData(BLUE "main:\t", &main, 0, elemSize);
	#endif
}

static size_t alignToRightmost(size_t vSize, size_t elemSize, size_t idx)
{
	size_t	aligned = idx - (idx % elemSize) + (elemSize - 1);
	if (aligned >= vSize)
		aligned = vSize - 1;
	return (aligned);
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

/// @brief binary search of lower insertion point for target val
/// @param main container to search in
/// @param ostart lower bound index
/// @param oend upper bound index
/// @param val target value
/// @param elemSize number of int in one elem
/// @return index ajusted to 0 or main.size in edge cases
size_t	PmergeMe::_binarySearch(t_vec& main, size_t ostart, size_t oend, int val, size_t elemSize)
{
	const size_t	n = main.size();
	int				start;
	int				end;
	size_t			span;
	size_t			mid;
	size_t			rightMostIdx;
	size_t			leftMostIdx;
	size_t			insertIdx;

	if (n == 0 || elemSize == 0 || ostart > oend)
		return 0;
	if (oend >= n)
		end = n - 1;
	start = alignToRightmost(main.size(), elemSize, ostart);
	end = alignToRightmost(main.size(), elemSize, oend);

	if (DEB)
	{
		printByPair(GREY "search:\t", &main, 0, elemSize);
		std::cout << "bin search between start = " << start << " and end = " << end << NC << "\n";
	}
	while (_isOngoingBissect(start, end))
	{
		span = (end - start) / elemSize;
		mid = start + (span / 2 * elemSize);
		if (_isLowerInsert(main[mid], val))
		{
			if (DEB)
			{
				std::cout << main[mid] << RED << " < " << CYAN << val << NC << " ... incrementing start to " << mid + elemSize << NC << "\n";
			}
			start = mid + elemSize;
			if (start > end)
				start = end;
		}
		else
		{
			if (DEB)
			{
				std::cout << main[mid] << BLUE << " >= " << CYAN << val << NC << " ... decrementing end to " << mid - elemSize << NC << "\n";
			}
			end = mid - elemSize;
			if (end < 0)
				end = 0;
		}
	}
	rightMostIdx = start;
	leftMostIdx = rightMostIdx - (elemSize - 1);
	if (_isUpperInsert(main[rightMostIdx], val))
	{
		if (DEB)
		{
			std::cout << "checking main[rightmost]=" << main[rightMostIdx] << BLUE << " > " << CYAN << val << NC << " ... setting insert idx to leftmost = " << leftMostIdx << NC << "\n";
		}
		insertIdx = leftMostIdx;
	}
	else
	{
		if (DEB)
		{
			std::cout << "checking main[rightmost]=" << main[rightMostIdx] << RED << " <= " << CYAN << val << NC << " ... setting insert idx to rightmost + 1 = " << rightMostIdx + 1 << NC << "\n";
		}
		insertIdx = rightMostIdx + 1;
	}
	return insertIdx;
}

/// @brief creates pairs (min, max)
/// @param data container containing pairs and possibly extra odd values
/// @param elemSize number of int in one elem
void	PmergeMe::_merge(t_vec* data, size_t elemSize, int depth)
{
	#ifdef DEBUG
	printTitle(BG_PURPLE, "Merge");
	printByPair(BLUE "before:\t", data, depth, elemSize);
	#else
	(void) depth;
	#endif
	size_t	pairSize = elemSize * 2;
	size_t	dataSize = data->size();

	if (data->size() < pairSize)
		return ;
	for (size_t i = 0; i + pairSize <= dataSize; i += pairSize)
	{
		t_vec::iterator itFirst = data->begin() + i + elemSize - 1;
		t_vec::iterator itSecond = data->begin() + i + elemSize + elemSize - 1;
		if (_isGreaterMerge(*itFirst, *itSecond))
		{
			t_vec::iterator maxStart = data->begin() + i;
			t_vec::iterator maxEnd = data->begin() + i + elemSize;
			t_vec::iterator minStart = data->begin() + i + elemSize;
			std::swap_ranges(maxStart, maxEnd, minStart);
		}
	}
	#ifdef DEBUG
	printByPair(BLUE "after:\t", data, depth, elemSize, true);
	#endif
}

/// @brief binary inserts pending + odd elems into main, first by bounding with Jacobstahl sequence, then for all the surface of main
/// @param data 
/// @param elemSize 
void	PmergeMe::_insert(t_vec* data, size_t elemSize, int depth)
{
	#ifdef DEBUG
	printTitle(BG_PURPLE, "Insert");
	printByPair(BLUE "data:\t", data, depth, elemSize);
	#else
	(void) depth;
	#endif
	t_vec					main;
	size_t					dataSize = data->size();
	size_t					nbElems = dataSize / elemSize;
	if (dataSize % 2 == 1 && elemSize == 1)
		++nbElems;
	size_t					pairSize = elemSize * 2;
	fillMain(main, data, elemSize, nbElems);

	(void) depth;

	size_t					k = 3;
	size_t					tk = _jacob[k];
	size_t					batchSize = tk - _jacob[k - 1];
	size_t					pendingNb = nbElems % 2 == 0 ? (nbElems - 2) / 2 : (nbElems - 2) / 2 + 1;
	size_t					i;
	int						toInsert;
	size_t					insertOffset;
	size_t					fromIndex;

	#ifdef DEBUG
	if (batchSize <= pendingNb)
		printColor(PURPLE, "Using Jacostahl sequence");
	#endif
	if (DEB)
	{
		std::cout << "pendingNb=" << pendingNb << std::endl;
		std::cout << "batchSize=" << batchSize << std::endl;
	}
	while (batchSize <= pendingNb)
	{
		i = getIndexOfLastNb((tk - 1) * 2, elemSize);
		#ifdef DEBUG
		printByPairWithPending("batch:\t", data, depth, elemSize, i, batchSize);
		#endif
		while (batchSize > 0 && i < data->size())
		{
			toInsert = (*data)[i];
			insertOffset = _binarySearch(main, elemSize - 1, (tk + 1) * elemSize, toInsert, elemSize);
			fromIndex = i - elemSize + 1;
			if (DEB)
			{
				printData(BLUE "main:\t", &main, depth, elemSize);
				std::cout << "wanting to insert elem at index " << fromIndex << " = " << toInsert << " into main a index " << insertOffset << std::endl;
			}
			insertElemAtPos(main, data, elemSize, fromIndex, insertOffset);
			i -= pairSize;
			--pendingNb;
			--batchSize;
		}
		++k;
		if (k > _jacobLen)
			throw std::out_of_range("trying to reach beyond registered elems of Jacobstahl sequence");
		tk = _jacob[k];
		batchSize = tk - _jacob[k - 1];
	}

	#ifdef DEBUG
	printData(BLUE "main:\t", &main, depth, elemSize);
	if (pendingNb > 0)
		printColor(PURPLE, "Inserting remaining elements");
	#endif
	if (k > 3)
		i = getIndexOfLastNb((tk - 1), elemSize);
	else
		i = getIndexOfLastNb(2, elemSize);
	if (DEB)
	{
		std::cout << "i=" << i << std::endl;
		std::cout << "pendingNb=" << pendingNb << std::endl;
	}
	while (pendingNb > 0)
	{
		toInsert = (*data)[i];
		insertOffset = _binarySearch(main, elemSize - 1, main.size() - 1, toInsert, elemSize);
		fromIndex = i - elemSize + 1;
		if (DEB)
		{
			printData(BLUE "main:\t", &main, depth, elemSize);
			std::cout << "wanting to insert elem at index " << fromIndex << " = " << toInsert << " into main a index " << insertOffset << std::endl;
		}
		insertElemAtPos(main, data, elemSize, fromIndex, insertOffset);
		--pendingNb;
		i += pairSize;
	}

	#ifdef DEBUG
	printData(BLUE "main:\t", &main, depth, elemSize);
	if (elemSize != 1 && data->size() % 2 == 1)
	{
		printColor(PURPLE, "Adding odd numbers");
	}
	#endif
	size_t oddNb = dataSize - (nbElems * elemSize);
	for (size_t i = oddNb; i > 0; --i)
	{
		main.push_back((*data)[dataSize - i]);
	}		
	*data = main;
	#ifdef DEBUG
	printData(GREEN "sorted:\t", &main, depth, elemSize);
	#endif
}
