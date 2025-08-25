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
	timeToMerge(0), timeToInsert(0), nbCompMerge(0), nbCompInsert(0), vec(), list(), vecUnsorted(), listUnsorted(), _jacob(PmergeMe::_generateJSequence()) {}

PmergeMe::PmergeMe(const PmergeMe& other) : 
	timeToMerge(0), timeToInsert(0), nbCompMerge(0), nbCompInsert(0), vec(other.vec), list(other.list), vecUnsorted(other.vecUnsorted), listUnsorted(other.listUnsorted), _jacob(other._jacob) {}

PmergeMe::~PmergeMe() {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
	if (this != &other)
	{
		vec = other.vec;
		list = other.list;
		vecUnsorted = other.vecUnsorted;
		listUnsorted = other.listUnsorted;
}
	return (*this);
}

/***************************************************************
*					🛠️ STATIC FUNCTIONS						*
***************************************************************/

static void	insertElemAtBack(t_vec& main, t_vec& data, size_t elemSize, size_t fromIndex)
{
	for (size_t i = 0; i < elemSize ; ++i, ++fromIndex)
	{
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

static void	insertElemAtBack(t_list& main, t_list& data, size_t elemSize, size_t fromIndex)
{
	t_list::iterator it = data.begin();
	std::advance(it, fromIndex);
	t_list::iterator endit = it;
	std::advance(endit, elemSize);
	std::copy(it, endit, std::back_inserter(main));
}

static void insertElemAtPos(t_list& main, t_list* data, size_t elemSize, size_t fromIndex, size_t insertOffset)
{
	t_list::iterator it = data->begin();
	std::advance(it, fromIndex);
	t_list::iterator endit = it;
	std::advance(endit, elemSize);
	t_list::iterator insit = main.begin();
	std::advance(insit, insertOffset);
	std::copy(it, endit, std::inserter(main, insit));
}

static void	eraseElemsAtFront(t_vec& v, size_t elemSize, size_t nbElems)
{
	t_vec::iterator endElem = v.begin();
	std::advance(endElem, elemSize * nbElems);
	v.erase(v.begin(), endElem);
}

/// @brief generate sorted container main (aka S) made of first elem of mins (aka B) and all elems of maxs (aka A)
/// @param main 
/// @param data 
/// @param elemSize 
static void	fillMainPending(t_vec& main, t_vec& pending, t_vec* data, size_t elemSize, size_t nbElem)
{
	(void) nbElem;
	size_t	dataSize = data->size();
	
	#ifdef DEBUG
	std::cout << PURPLE << "\nFilling main....\n" << NC;
	#endif

	for (size_t i = 0; i <= dataSize - elemSize; i += elemSize)
	{
		if (i % (elemSize * 2) == 0)
			insertElemAtBack(pending, *data, elemSize, i);
		else
			insertElemAtBack(main, *data, elemSize, i);
	}
}

/// @brief generate sorted container main (aka S) made of first elem of mins (aka B) and all elems of maxs (aka A)
/// @param main 
/// @param data 
/// @param elemSize 
static void	fillMain(t_list& main, t_list* data, size_t elemSize)
{
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
	std::cout << elemSize << "\n";
	#endif
	for (size_t i = elemSize; i <= dataSize - elemSize; i += pairSize)
	{
		insertElemAtBack(main, *data, elemSize, i);
	}
	#ifdef DEBUG
	printData(BLUE "main:\t", &main, 0, elemSize);
	#endif
}

/// @brief used for binary search to be sure that the index is that of lead value in elem
/// @param vSize container size
/// @param elemSize 
/// @param idx 
/// @return aligned index
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

void	PmergeMe::initContainers(int ac, char **av)
{
	int		val;
	char	*end;

	for (int i = 0; i < ac; i++)
	{
		val = static_cast<int>(std::strtol(av[i], &end, 10));
		vec.push_back(val);
		vecUnsorted.push_back(val);
		list.push_back(val);
		listUnsorted.push_back(val);
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
size_t	PmergeMe::_binarySearch(t_vec& main, size_t start, size_t end, int val, size_t elemSize)
{
	const size_t	n = main.size();
	size_t			midElem;
	size_t			mid;


	if (n == 0 || elemSize == 0 || start > end)
		return 0;
	size_t nbElems = main.size() / elemSize;
	if (end > nbElems)
	{
		end = nbElems;
	}
	#ifdef DEBUG
	if (DEB)
		std::cout << BG_YELLOW  << "area of comp = " << end - start << " between indexes " << start << " and " << end << " of main" << NC << std::endl;
	#endif
	while (_isOngoingBissect(start, end))
	{
		midElem = (start + end) / 2;
		mid = midElem * elemSize + (elemSize - 1);
		#ifdef DEBUG
		if (DEB)
			std::cout << "bin search " << CYAN << val << NC << " between startElem #" << start << BLUE << " midElem=" << midElem << NC << " and endElem #= " << end << NC << "\n";
		#endif
		if (_isLowerInsert(main[mid], val))
		{
			start = midElem + 1;
			#ifdef DEBUG
			if (DEB)
				std::cout << main[mid] << RED << " < " << CYAN << val << NC << " ... incrementing startElem to " << start << " and endElem=" << end << NC << "\n";
			#endif
		}
		else
		{
			end = midElem;
			#ifdef DEBUG
			if (DEB)
				std::cout << main[mid] << BLUE << " >= " << CYAN << val << NC << " ... decrementing endElem to " << end << " and startElem= " << start << NC << "\n";
			#endif
		}
	}
	#ifdef DEBUG
	if (DEB)
		std::cout << CYAN << "insert at elemIndex " << start << NC << std::endl;
	#endif
	return start * elemSize;
}

int	PmergeMe::_getValAtIndexInsert(t_list& l, size_t index)
{
	std::list<int>::iterator	it = l.begin();
	std::advance(it, index);
	return (*it);
}

int	PmergeMe::_getValAtIndexInsertOffset(t_list* l, size_t startIndex, int offset)
{
	(void) offset;
	std::list<int>::iterator	it = l->begin();
	std::advance(it, startIndex);
	return (*it);
}

/// @brief binary search of lower insertion point for target val
/// @param main container to search in
/// @param ostart lower bound index
/// @param oend upper bound index
/// @param val target value
/// @param elemSize number of int in one elem
/// @return index ajusted to 0 or main.size in edge cases
size_t	PmergeMe::_binarySearch(t_list& main, size_t ostart, size_t oend, int val, size_t elemSize)
{
	const size_t	n = main.size();
	int				start;
	int				end;
	size_t			span;
	size_t			mid;
	size_t			rightMostIdx;
	size_t			leftMostIdx;
	size_t			insertIdx;
	int				midVal;
	int				rightMostVal;

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
		midVal = _getValAtIndexInsert(main, mid);
		if (_isLowerInsert(midVal, val))
		{
			if (DEB)
			{
				std::cout << midVal << RED << " < " << CYAN << val << NC << " ... incrementing start to " << mid + elemSize << NC << "\n";
			}
			start = mid + elemSize;
			if (start > end)
				start = end;
		}
		else
		{
			if (DEB)
			{
				std::cout << midVal << BLUE << " >= " << CYAN << val << NC << " ... decrementing end to " << mid - elemSize << NC << "\n";
			}
			end = mid - elemSize;
			if (end < 0)
				end = 0;
		}
	}
	rightMostIdx = start;
	leftMostIdx = rightMostIdx - (elemSize - 1);
	rightMostVal = _getValAtIndexInsert(main, rightMostIdx);
	if (_isUpperInsert(rightMostVal, val))
	{
		if (DEB)
		{
			std::cout << "checking main[rightmost]=" << rightMostVal << BLUE << " > " << CYAN << val << NC << " ... setting insert idx to leftmost = " << leftMostIdx << NC << "\n";
		}
		insertIdx = leftMostIdx;
	}
	else
	{
		if (DEB)
		{
			std::cout << "checking main[rightmost]=" << rightMostVal << RED << " <= " << CYAN << val << NC << " ... setting insert idx to rightmost + 1 = " << rightMostIdx + 1 << NC << "\n";
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

/// @brief creates pairs (min, max)
/// @param data container containing pairs and possibly extra odd values
/// @param elemSize number of int in one elem
void	PmergeMe::_merge(t_list* data, size_t elemSize, int depth)
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
		t_list::iterator itFirst = data->begin();
		std::advance(itFirst, i + elemSize - 1);
		t_list::iterator itSecond = data->begin();
		std::advance(itSecond, i + elemSize + elemSize - 1);
		if (_isGreaterMerge(*itFirst, *itSecond))
		{
			t_list::iterator maxStart = data->begin();
			std::advance(maxStart, i);
			t_list::iterator maxEnd = data->begin();
			std::advance(maxEnd, i + elemSize);
			t_list::iterator minStart = data->begin();
			std::advance(minStart, i + elemSize);
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
	size_t					pairSize = elemSize * 2;
	#else
	(void) depth;
	#endif

	t_vec					main;
	t_vec					pending;
	size_t					dataSize = data->size();
	size_t					nbElems = dataSize / elemSize;

	fillMainPending(main, pending, data, elemSize, nbElems);

	#ifdef DEBUG
		printColor(PURPLE, "Building main and pending");
	#endif

	t_vec::iterator endElem = pending.begin();
	std::advance(endElem, elemSize);
	main.insert(main.begin(), pending.begin(), endElem);
	pending.erase(pending.begin(), endElem);

	#ifdef DEBUG
		printMain(BLUE "main:\t", &main, depth, elemSize);
	#endif

	size_t					k = 3;
	size_t					tk = _jacob[k];
	size_t					boundIndex;
	size_t					batchSize = tk - _jacob[k - 1];
	size_t					pendingNb = pending.size() / elemSize;
	int						toInsert;
	size_t					insertOffset;
	size_t					fromIndex;
	size_t					nbPairsinMain = 1;
	size_t					nbInserted = 0;
	size_t					iPend;
	#ifdef DEBUG
	size_t					i = 0;
	size_t					prevI = elemSize - 1;
	if (batchSize < pendingNb)
		printTitle(PURPLE, "Using Jacostahl sequence");
	#endif

	while (batchSize < pendingNb)
	{
		#ifdef DEBUG
		if (prevI == elemSize - 1)
			i = prevI + pairSize * 2;
		else
			i = prevI + batchSize * pairSize;
		prevI = i;
		printTitle(YELLOW, "start of batch");
		printByPairWithPending("batch:\t", data, depth, elemSize, i, batchSize);
		#endif

		size_t j = 0;
		iPend = batchSize * elemSize - 1;
		while (j < batchSize)
		{
			toInsert = pending[iPend];
			boundIndex = (nbPairsinMain * 2) + batchSize - 1;

			#ifdef DEBUG
			if (DEB)
				std::cout << "boundIndex=" << boundIndex << "\n";
			#endif

			insertOffset = _binarySearch(main, 0, boundIndex, toInsert, elemSize);
			fromIndex = iPend - (elemSize - 1);
			insertElemAtPos(main, &pending, elemSize, fromIndex, insertOffset);

			#ifdef DEBUG
			printMain(BLUE "main:\t", &main, depth, elemSize);
			if (!isSortedAsc(main, elemSize))
			{
				std::cout << BG_RED << "not sorted !!!!" << NC << std::endl;
			}
			i -= pairSize;
			#endif
	
			iPend -= elemSize;
			++j;
			--pendingNb;
			++nbInserted;
		}

		eraseElemsAtFront(pending, elemSize, batchSize);
		
		nbPairsinMain += batchSize;
		++k;
		if (k > _jacobLen)
			throw std::out_of_range("trying to reach beyond registered elems of Jacobstahl sequence");
		tk = _jacob[k];
		batchSize = tk -_jacob[k - 1];
	}

	#ifdef DEBUG
	if (pendingNb > 0)
		printTitle(PURPLE, "Inserting remaining elements");
	#endif

	nbInserted = 0;
	while (pendingNb > 0)
	{
		toInsert = pending[elemSize - 1];
		boundIndex = nbPairsinMain * 2 + nbInserted * 2 + 1;
		if (main.size() / elemSize < boundIndex)
		{
			boundIndex = main.size() / elemSize;
		}
		#ifdef DEBUG
		if (DEB)
			std::cout << "boundIndex=" << boundIndex << "\n";
		#endif

		insertOffset = _binarySearch(main, 0, boundIndex, toInsert, elemSize);
		fromIndex = 0;
		insertElemAtPos(main, &pending, elemSize, fromIndex, insertOffset);

		#ifdef DEBUG
		printMain(BLUE "main:\t", &main, depth, elemSize);
		if (!isSortedAsc(main, elemSize))
		{
			std::cout << BG_RED << "not sorted !!!!" << NC << std::endl;
		}
		#endif

		eraseElemsAtFront(pending, elemSize, 1);
		--pendingNb;
		++nbInserted;
		#ifdef DEBUG
		i += pairSize;
		#endif
	}

	#ifdef DEBUG
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
	printData(BLUE "sort:\t", &main, depth, elemSize);
	#endif
}

/// =============================================================================
///	LIST
/// =============================================================================


/// @brief binary inserts pending + odd elems into main, first by bounding with Jacobstahl sequence, then for all the surface of main
/// @param data 
/// @param elemSize 
void	PmergeMe::_insert(t_list* data, size_t elemSize, int depth)
{
	#ifdef DEBUG
	printTitle(BG_PURPLE, "Insert");
	printByPair(BLUE "data:\t", data, depth, elemSize);
	#else
	(void) depth;
	#endif
	t_list					main;
	size_t					dataSize = data->size();
	size_t					nbElems = dataSize / elemSize;
	size_t					pairSize = elemSize * 2;
	fillMain(main, data, elemSize);

	(void) depth;

	size_t					k = 3;
	size_t					tk = _jacob[k];
	size_t					boundIndex;
	size_t					batchSize = tk - _jacob[k - 1];
	size_t					dataSizeMinusOddAndFirstPair = dataSize - (dataSize % elemSize) - (elemSize * 2);
	size_t					pendingNb = dataSizeMinusOddAndFirstPair / (elemSize * 2);
	if (dataSizeMinusOddAndFirstPair % (elemSize * 2) != 0)
		pendingNb += 1;
	size_t					i;
	int						toInsert;
	size_t					insertOffset;
	size_t					fromIndex;
	size_t					prevI = elemSize - 1;

	#ifdef DEBUG
	if (batchSize <= pendingNb)
		printColor(PURPLE, "Using Jacostahl sequence");
	#endif
	if (DEB)
	{
		std::cout << "nbElems=" << nbElems << std::endl;
		std::cout << "dataSizeMinusOddAndFirstPair=" << dataSizeMinusOddAndFirstPair << std::endl;
		std::cout << "pendingNb=" << pendingNb << std::endl;
		std::cout << "batchSize=" << batchSize << std::endl;
	}
	size_t	nbInserted = 0;
	while (batchSize < pendingNb)
	{
		if (prevI == elemSize - 1)
			i = prevI + pairSize * 2;
		else
			i = prevI + (batchSize * pairSize);
		prevI = i;
		if (DEB)
		{
			std::cout << "k= " << k << "\n";
			std::cout << "tk= " << tk << "\n";
			std::cout << "pendingNb=" << pendingNb << NC << std::endl;
			std::cout << "batchSize=" << batchSize << std::endl;
			std::cout << "i=" << i << std::endl;
			std::cout << "upperBound=" << boundIndex << std::endl;
		}
		#ifdef DEBUG
		printByPairWithPending("batch:\t", data, depth, elemSize, i, batchSize);
		#endif
		while (batchSize > 0 && i < data->size())
		{
			if (i == prevI)
				toInsert = _getValAtIndexInsert(*data, i);
			else
				toInsert = _getValAtIndexInsertOffset(data, i, -pairSize);
			boundIndex = std::min(main.size() + 1, (tk + nbInserted + 1) * elemSize);
			insertOffset = _binarySearch(main, elemSize - 1, boundIndex, toInsert, elemSize);
			fromIndex = i - elemSize + 1;
			if (DEB)
			{
				std::cout << "wanting to insert elem at index " << fromIndex << " = " << toInsert << " into main a index " << insertOffset << std::endl;
			}
			insertElemAtPos(main, data, elemSize, fromIndex, insertOffset);
			if (DEB)
			{
				printData(BLUE "main:\t", &main, depth, elemSize);
				if (!isSortedAsc(main, elemSize))
				{
					std::cout << BG_RED << "not sorted !!!!" << NC << std::endl;
				}
			}
			i -= pairSize;
			--pendingNb;
			--batchSize;
			++nbInserted;
		}
		++k;
		if (k > _jacobLen)
			throw std::out_of_range("trying to reach beyond registered elems of Jacobstahl sequence");
		tk = _jacob[k];
		batchSize = tk -_jacob[k - 1];
	}

	#ifdef DEBUG
	printData(BLUE "main:\t", &main, depth, elemSize);
	if (pendingNb > 0)
		printColor(PURPLE, "Inserting remaining elements");
	#endif
	i = prevI + (pairSize);
	if (DEB)
	{
		std::cout << "k=" << k << std::endl;
		std::cout << "tk=" << tk << std::endl;
		std::cout << "i=" << i << std::endl;
		std::cout << "pendingNb=" << pendingNb << std::endl;
		std::cout << "nbInserted=" << nbInserted << std::endl;
	}
	while (pendingNb > 0)
	{
		if (i == prevI + pairSize)
			toInsert = _getValAtIndexInsertOffset(data, i, pairSize);
		else
			toInsert = _getValAtIndexInsertOffset(data, i, pairSize);
		boundIndex = std::min(main.size() + 1, (tk + nbInserted + 1) * elemSize);
		insertOffset = _binarySearch(main, elemSize - 1, boundIndex, toInsert, elemSize);		
		fromIndex = i - elemSize + 1;
		if (DEB)
		{
			printData(BLUE "main:\t", &main, depth, elemSize);
			std::cout << "wanting to insert elem at index " << fromIndex << " = " << toInsert << " into main a index " << insertOffset << std::endl;
		}
		insertElemAtPos(main, data, elemSize, fromIndex, insertOffset);
		--pendingNb;
		++nbInserted;
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
	if (DEB)
			std::cout << "oddnb= " << oddNb << std::endl;
	if (DEB)
	{
		std::cout << "dataSize = " << dataSize << "\n";
		std::cout << "nbElems = " << nbElems << "\n";
		std::cout << "oddnb = " << oddNb << "\n";
		std::cout << "i = " << i << "\n";
	}
	t_list::iterator oddit = data->begin();
	std::advance(oddit, dataSize - 1 - oddNb);
	for (size_t i = 0; i < oddNb; ++i)
	{
		std::advance(oddit, 1);
		if (DEB)
			std::cout << CYAN << "i= " << i << "*oddit=" << *oddit << std::endl;
		main.push_back(*oddit);
	}

	*data = main;
	#ifdef DEBUG
	printData(GREEN "sorted:\t", &main, depth, elemSize);
	#endif
}
