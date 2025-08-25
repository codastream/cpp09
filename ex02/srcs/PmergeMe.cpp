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

static int	getValAtIndexInsert(t_list& l, size_t index)
{
	std::list<int>::iterator	it = l.begin();
	std::advance(it, index);
	return (*it);
}

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

static void	eraseElemsAtFront(t_list& l, size_t elemSize, size_t nbElems)
{
	t_list::iterator endElem = l.begin();
	std::advance(endElem, elemSize * nbElems);
	l.erase(l.begin(), endElem);
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
static void	fillMainPending(t_vec& main, t_vec& pending, t_vec* data, size_t elemSize)
{
	size_t	dataSize = data->size();
	
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
static void	fillMainPending(t_list& main, t_list& pending, t_list* data, size_t elemSize)
{
	size_t	dataSize = data->size();
	
	for (size_t i = 0; i <= dataSize - elemSize; i += elemSize)
	{
		if (i % (elemSize * 2) == 0)
			insertElemAtBack(pending, *data, elemSize, i);
		else
			insertElemAtBack(main, *data, elemSize, i);
	}
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
/// @param start lower bound index
/// @param end upper bound index
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

/// @brief binary search of lower insertion point for target val
/// @param main container to search in
/// @param start lower bound index
/// @param end upper bound index
/// @param val target value
/// @param elemSize number of int in one elem
/// @return index ajusted to 0 or main.size in edge cases
size_t	PmergeMe::_binarySearch(t_list& main, size_t start, size_t end, int val, size_t elemSize)
{
	const size_t	n = main.size();
	size_t			midElem;
	size_t			mid;
	int				midVal;

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
		midVal = getValAtIndexInsert(main, mid);
		if (_isLowerInsert(midVal, val))
		{
			start = midElem + 1;
			#ifdef DEBUG
			if (DEB)
				std::cout << midVal << RED << " < " << CYAN << val << NC << " ... incrementing startElem to " << start << " and endElem=" << end << NC << "\n";
			#endif
		}
		else
		{
			end = midElem;
			#ifdef DEBUG
			if (DEB)
				std::cout << midVal << BLUE << " >= " << CYAN << val << NC << " ... decrementing endElem to " << end << " and startElem= " << start << NC << "\n";
			#endif
		}
	}
	#ifdef DEBUG
	if (DEB)
		std::cout << CYAN << "insert at elemIndex " << start << NC << std::endl;
	#endif
	return start * elemSize;
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

/// @brief combine max elems, min elems, odd elems and remaining numbers into a sorted sequence
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

	#ifdef DEBUG
		printColor(PURPLE, "Building main and pending");
	#endif
	fillMainPending(main, pending, data, elemSize);

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
			boundIndex = main.size() / elemSize;

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

/// @brief combine max elems, min elems, odd elems and remaining numbers into a sorted sequence
/// @param data 
/// @param elemSize 
void	PmergeMe::_insert(t_list* data, size_t elemSize, int depth)
{
	#ifdef DEBUG
	printTitle(BG_PURPLE, "Insert");
	printByPair(BLUE "data:\t", data, depth, elemSize);
	size_t					pairSize = elemSize * 2;
	#else
	(void) depth;
	#endif
	t_list					main;
	t_list					pending;
	size_t					dataSize = data->size();
	size_t					nbElems = dataSize / elemSize;

	#ifdef DEBUG
		printColor(PURPLE, "Building main and pending");
	#endif
	fillMainPending(main, pending, data, elemSize);

	t_list::iterator endElem = pending.begin();
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

		size_t	j = 0;
		iPend = batchSize * elemSize - 1;
		while (j < batchSize)
		{
			toInsert = getValAtIndexInsert(pending, iPend);
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
		printColor(PURPLE, "Inserting remaining elements");
	#endif

	nbInserted = 0;
	while (pendingNb > 0)
	{
		toInsert = getValAtIndexInsert(pending, elemSize - 1);
		boundIndex = nbPairsinMain * 2 + nbInserted * 2 + 1;
		if (main.size() / elemSize < boundIndex)
			boundIndex = main.size() / elemSize;
	
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

	t_list::iterator oddit = data->begin();
	std::advance(oddit, dataSize - 1 - oddNb);
	for (size_t i = 0; i < oddNb; ++i)
	{
		std::advance(oddit, 1);
		main.push_back(*oddit);
	}

	*data = main;
	#ifdef DEBUG
	printData(GREEN "sort:\t", &main, depth, elemSize);
	#endif
}
