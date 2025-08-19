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

// inline swapPairs(t_vec::iterator itMax, t_vec::iterator itMin, size_t elemSize)
// {
// 	for (size_t i = 0; i < elemSize; i++)
// 		std::swap(*itMax--, *itMin--);
// }

static size_t getIndexOfLastNb(size_t elemIndex, size_t elemSize)
{
	size_t res = (elemIndex + 1) * elemSize - 1;
	std::cout << "looking for index of last for elem at # " << elemIndex << " and of size " << elemSize << "\n";
	return res;
}

static void	insertElemAtBack(t_vec& main, t_vec& data, size_t elemSize, size_t fromIndex)
{
	for (size_t i = 0; i < elemSize && data[fromIndex]; ++i, ++fromIndex)
	{
		main.push_back(data[fromIndex]);
	}
}

static void insertElemAtPos(t_vec& main, t_vec* data, size_t elemSize, size_t fromIndex, size_t insertOffset)
{
	for (size_t i = 0; i < elemSize && fromIndex < data->size(); ++i, ++fromIndex)
	{
		std::cout << "adding " << (*data)[fromIndex] << " from index " << fromIndex << "\n";
		main.insert(main.begin() + insertOffset + i, (*data)[fromIndex]);
		printData("main :", &main, 0, elemSize);
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
	std::cout << PURPLE << "\nFilling main....\n" << NC;
	std::cout << "pair size " << pairSize << std::endl;
	
	insertElemAtBack(main, *data, elemSize, 0);
	// std::cout << "adding to main " << data[elemSize - 1] << "\n";
	printData("adding max elems:", &main, 0, elemSize);
	for (size_t i = elemSize; i <= dataSize - elemSize; i += pairSize)
	{
		std::cout << "adding one elem from data index " << i << " into main\n"; 
		insertElemAtBack(main, *data, elemSize, i);
		printData("main :", &main, 0, elemSize);
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

size_t	PmergeMe::_binarySearch(t_vec& main, size_t start, size_t end, int val, size_t elemSize)
{
	size_t	insertIdx = 0;
	size_t nbElemsBetweenStartEnd = (end - start) / elemSize;
	int mid = start + (nbElemsBetweenStartEnd / 2 * elemSize);
	while (_isOngoingBissect(start, end))
	{
		nbElemsBetweenStartEnd = (end - start) / elemSize;
		mid = start + (nbElemsBetweenStartEnd / 2 * elemSize);
		std::cout << "searching " << CYAN << val << NC << " between start=" << start << " and end=" << end << " mid = " << mid << std::endl;
		std::cout << "nbElems between start end " << nbElemsBetweenStartEnd << "\n";
		std::cout << "now checking main[" << mid << "]=" << main[mid] << "...";
		if (_isMidTooLowInsert(main[mid], val))
		{
			start = mid + elemSize;
			insertIdx = start;
			// nbElemsBetweenStartEnd = (end - start) / elemSize;
			// mid = start + (nbElemsBetweenStartEnd / 2 * elemSize);
			std::cout << RED << "below" NC << val << "=> setting start to " << start << " insert index now " << insertIdx << "\n";
		}
		else
		{
			// what if mid >= elemScize
			end = mid;
			// nbElemsBetweenStartEnd = (end - start) / elemSize;
			// mid = start + (nbElemsBetweenStartEnd / 2 * elemSize);
			std::cout << BLUE << "above" << NC << val << "=> setting end to " << end << " insert index now " << insertIdx << "\n";
		}
	}
	std::cout << "for val " << val << " now bounded by start=" << start << " and end=" << end << " insertIdx = " << insertIdx << std::endl;
	return insertIdx;
}

/// @brief creates pairs (min, max)
/// @param data 
/// @param elemSize 
void	PmergeMe::_merge(t_vec* data, size_t elemSize, int depth)
{
	#ifdef DEBUG
	printByPair(BLUE "before merge:\t", data, depth, elemSize);
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
			std::cout << *itFirst << " > " << *itSecond << "...\n";
			t_vec::iterator maxStart = data->begin() + i;
			t_vec::iterator maxEnd = data->begin() + i + elemSize;
			t_vec::iterator minStart = data->begin() + i + elemSize;
			std::cout << "moving max elem (from " << *maxStart << " to " <<  *maxEnd << ")" << std::endl; 
			std::swap_ranges(maxStart, maxEnd, minStart);
		}
	}
	#ifdef DEBUG
	printByPair(BLUE "after merge:\t", data, depth, elemSize, true);
	#endif
}

/// @brief binary inserts pending + odd elems into main, first by bounding with Jacobstahl sequence, then for all the surface of main
/// @param data 
/// @param elemSize 
void	PmergeMe::_insert(t_vec* data, size_t elemSize, int depth)
{
	#ifdef DEBUG
	printData(BLUE "data:\t\t", data, depth, elemSize);
	#else
	(void) depth;
	#endif
	t_vec					main;
	size_t					nbElems = data->size() / elemSize;
	size_t					pairSize = elemSize * 2;
	fillMain(main, data, elemSize, nbElems);
	#ifdef DEBUG
	printData(BLUE "main:\t\t", &main, depth, elemSize);
	#else
	(void) depth;
	#endif

	size_t					k = 3;
	size_t					tk = _jacob[k];
	size_t					batchSize = tk - _jacob[k - 1];
	size_t					pendingNb = nbElems % 2 == 0 ? (nbElems - 2) / 2 : (nbElems - 2) / 2 + 1;
	size_t					i;
	int						toInsert;
	size_t					insertOffset;
	size_t					fromIndex;
	
	std::cout << PURPLE << "\ninsertion.." << NC << "\n";
	while (batchSize <= pendingNb)
	{
		std::cout << PURPLE << "\ninsert Jacobstahl for tk = " << k << NC << "\n";

		i = getIndexOfLastNb((tk - 1) * 2, elemSize);
		std::cout << BOLD_ON << "starting batch from index " << i << BOLD_OFF "\n";
		while (batchSize > 0 && i < data->size())
		{
			toInsert = (*data)[i];
			std::cout << "pending nb is " << pendingNb << "\n";
			std::cout << "looking at data index " << i << "\n";
			std::cout << CYAN << "trying to Insert " << toInsert << NC << "\n";
			insertOffset = _binarySearch(main, elemSize - 1, tk * elemSize + elemSize - 1, toInsert, elemSize);
			std::cout << "... should be inserted at i " << insertOffset << std::endl;
			fromIndex = i - elemSize + 1;
			insertElemAtPos(main, data, elemSize, fromIndex, insertOffset);
			i -= pairSize;
			--pendingNb;
			--batchSize;
		}
		++k;
		if (k > _jacobLen)
			throw std::out_of_range("trying to reach beyond registered elems of Jacobstahl sequence");
		tk = _jacob[k];
		std::cout << "\nk is now " << BLUE << k << NC " and tk " << CYAN << tk << "\n";
		batchSize = tk - _jacob[k - 1];
	}

	std::cout << PURPLE << "\ninsert remaining elems.." << NC << "\n";
	if (k > 3)
		i = getIndexOfLastNb((tk + 1), elemSize);
	else
		i = getIndexOfLastNb(2, elemSize);
	#ifdef DEBUG
	printData(BLUE "main before insert remaining:\t", &main, depth, elemSize);
	printData(BLUE "data before insert remaining:\t", data, depth, elemSize);
	#endif
	while (pendingNb > 0)
	{
		toInsert = (*data)[i];

		std::cout << "pending nb is " << pendingNb << "\n";
		std::cout << "looking at data index " << i << "\n";
		std::cout << "trying to Insert " << toInsert << "\n";

		insertOffset = _binarySearch(main, elemSize - 1, main.size() - 1, toInsert, elemSize);
		std::cout << "... should be inserted at i " << insertOffset << std::endl;
		fromIndex = i - elemSize + 1;
		insertElemAtPos(main, data, elemSize, fromIndex, insertOffset);
		--pendingNb;
		i += elemSize;
	}

	std::cout << PURPLE << "adding odd nb to data\n" << NC;
	if (elemSize != 1 && data->size() % 2 == 1)
		main.push_back((*data)[data->size() - 1]);
	*data = main;
	#ifdef DEBUG
	printData(BLUE "sorted:\t\t", &main, depth, elemSize);
	#endif
}
