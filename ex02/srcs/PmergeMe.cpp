#include "PmergeMe.hpp"
#include "util.hpp"

/************************************************************
*				🥚 CONSTRUCTORS & DESTRUCTOR				*
************************************************************/

static std::vector<int>	generateJSequence(int size);
static void _printVec(std::vector<int>& v, const std::string& msg, int depth, int outlineIdxStart, int outlineIdxend, const char* outlineColor);

PmergeMe::PmergeMe(void) : _vec(), _compMerge(0), _compInsert(0), _timeToMerge(0), _timeToInsert(0), _jacob(generateJSequence(21)) {}

PmergeMe::PmergeMe(const PmergeMe& inst) : _vec(inst._vec), _compMerge(0), _compInsert(0), _timeToMerge(0), _timeToInsert(0), _jacob(inst._jacob) {}

PmergeMe::~PmergeMe(void) {}

/************************************************************
*				➕ OPERATORS									*
************************************************************/

PmergeMe& PmergeMe::operator=(const PmergeMe& inst) 
{
	if (this != &inst)
	{
		_vec = inst._vec;
	}
	return (*this);
}

/*************************************************************
*		        🛠️ FUNCTIONS								*
*************************************************************/

static void _printVec(std::vector<int>& v, const std::string& msg, int depth, int outlineIdxStart, int outlineIdxend, const char* outlineColor)
{
	std::string color = YELLOW;
	if (!DEBUG)
		return ;
	for (int i = 0; i < depth; i++)
		std::cout << "  ";
	std::cout << BLUE << std::left << std::setw(16) << msg << NC;
	for (int i = 0; i < depth; i++)
		std::cout << "\t";
	int i = 0;
	for (std::vector<int>::const_iterator cit = v.begin(); cit != v.end(); ++cit)
	{
		if (outlineIdxStart != -1 && i >= outlineIdxStart && i <= outlineIdxend)
			color = outlineColor;
		else
			color = YELLOW;
		std::cout << color << std::left << std::setw(8) << *cit << NC;
		i++;
	}
	std::cout << std::endl;
}

#if PRINTSTEP
static void _printPairs(std::vector<int>& v, std::vector<std::pair<int, int> >& pairs, int depth)
{
	size_t psize = pairs.size();
	for (int i = 0; i < depth * 2; i++)
		std::cout << " ";
	std::cout << BLUE << std::left << std::setw(16) << "sorted pairs" << NC;
	for (int i = 0; i < depth; i++)
		std::cout << "\t";
	for (size_t i = 0 ; i < psize; ++i)
	{
		if (pairs[i].first != v[i * 2])
		{
			std::cout << std::left << BPURPLE << std::left << std::setw(8) << pairs[i].first << PURPLE << std::setw(8) << pairs[i].second << YELLOW << NC;
		}
		else
			std::cout  << std::left << YELLOW << std::left << std::setw(8) << pairs[i].first << std::setw(8) << pairs[i].second << YELLOW << NC;
	}
	std::cout << std::endl;
}
#endif

static std::vector<int>	generateJSequence(int size)
{
	std::vector<int> v;
	v.push_back(0);
	if (size <= 0)
		return v;
	v.push_back(1);
	for(int i = 2; i < size; ++i)
	{
		v.push_back(v[i - 1] + 2 * v[i - 2]);
	}
	return v;
}

/// @brief 
/// @param v 
/// @param val 
int	PmergeMe::_binarySearchIndex(std::vector<int>&v, int val, int tk)
{
	int start = 0;
	int end = tk;
	if (v.size() == 0)
		return 0;
	else if (end == 1)
	{
		if (val > v[0])
		{
			++_compInsert;
			return 1;
		}
		else
			return 0;
	}
	while (start <= end)
	{
		++_compInsert;
		int mid = start + (end - start) / 2;
		if (v[mid] < val)
		{
			++_compInsert;
			start = mid + 1;
		}
		else if (v[mid] > val)
		{
			++_compInsert;
			end = mid - 1;
		}
		else
		{
			return mid;
		}
	}
	return (start);
}

void	PmergeMe::doSort()
{
	int	n;

	n = _vec.size();
	std::vector<int> sorted;
	_printVec(_vec, "input", 0, -1, -1, YELLOW);

	_sortVec(_vec, &sorted, 0);

	_printVec(sorted, "sorted", 0, -1, -1, YELLOW);

	printTitle(BLUEBACK, "RESULTS");
	printTitle(BLUE, "Input size (n)");
	std::cout << n << std::endl;
	printTitle(BLUE, "Expected complexity");
	std::cout << BLUE << "theoretical best = O (n log n −1.4427n)\t" << NC << n * std::log(n) - 1.4427 * n << std::endl;
	printTitle(BLUE, "Nb of comparisons");
	std::cout << BLUE << "merge\t" << NC << _compMerge << std::endl;
	std::cout << BLUE << "insert\t" << NC << _compInsert << std::endl;
	std::cout << BBLUE << "total\t" << NC << _compMerge + _compInsert << std::endl;
	printTitle(BLUE, "Compute time");
	std::cout << BLUE << "merge\t" << NC << std::fixed << std::setprecision(5) << _timeToMerge << " sec" << std::endl;
	std::cout << BLUE << "insert\t" << NC << std::fixed << std::setprecision(5) << _timeToInsert << " sec" << std::endl;
	std::cout << BBLUE << "total\t" << NC << std::fixed << std::setprecision(5) << _timeToMerge + _timeToInsert << " sec" << std::endl;
}

bool	PmergeMe::_checkEndOfRecursion(std::vector<int>& v, std::vector<int>* sorted, int depth)
{
	(void) depth;
	clock_t	start = clock();
	if (v.size() <= 1)
	{
		*sorted = v;
		return true;
	}
	if (v.size() == 2)
	{
		if (v[0] > v[1])
		{
			std::swap(v[0], v[1]);
		}
		_compMerge++;
		*sorted = v;
		return true;
	}
	clock_t	end = clock();
	_timeToMerge += double(end - start) / double(CLOCKS_PER_SEC);
	return false;
}

void	PmergeMe::_sortPairs(std::vector<int>&v, std::vector<std::pair<int, int> >&	pairs, int depth)
{
	#if PRINTSTEP
	printcoldep(depth, IWHITE, "step 1 - making and sorting pairs");
	_printVec(v, "before presort", depth, -1, -1, YELLOW);
	#else
	(void) depth;
	#endif
	clock_t start = clock();
	std::vector<int> 					oldv = v; // only for printpairs
	for (size_t i = 0; i + 1 < v.size(); i += 2)
	{
		if (v[i] < v[i + 1])
			std::swap(v[i], v[i + 1]);
		_compMerge++;
		pairs.push_back(std::make_pair(v[i], v[i + 1]));
	}
	clock_t	end = clock();
	_timeToMerge += double(end - start) / double(CLOCKS_PER_SEC);
	#if PRINTSTEP
	_printPairs(oldv, pairs, depth);
	#endif
}

void	PmergeMe::_sortBiggest(size_t vSize, std::vector<int>& main, std::vector<std::pair<int, int> >&	pairs, int depth)
{
	#if PRINTSTEP
	printcoldep(depth, IWHITE, "step 2 - recursively sorting the biggest elements");
	#endif
	clock_t	start = clock();
	std::vector<int> vBig;
	vBig.reserve((vSize + 1) / 2);
	for (size_t i = 0; i < pairs.size(); ++i)
	{
		vBig.push_back(pairs[i].first);
	};
	clock_t	end = clock();
	_timeToMerge += double(end - start) / double(CLOCKS_PER_SEC);
	#if PRINTSTEP
	_printVec(vBig, "biggest", depth, -1, -1, YELLOW);
	#endif
	_sortVec(vBig, &main, depth + 1);
}

void	PmergeMe::_createPending(std::vector<int>& v, std::vector<int>& pending, std::vector<std::pair<int, int> >&	pairs, int depth)
{
	#if PRINTSTEP
	printcoldep(depth, IWHITE, "step 3 - pending chain");
	#else
	(void) depth;
	#endif
	clock_t	start = clock();
	pending.reserve(v.size() / 2);
	for (size_t i = 0; i < pairs.size(); ++i)
	{
		pending.push_back(pairs[i].second);
	};
	if (v.size() % 2 == 1)
		pending.push_back(v.back());
	clock_t	end = clock();
	_timeToMerge += double(end - start) / double(CLOCKS_PER_SEC);
	#if PRINTSTEP
	_printVec(pending, "pending", depth, -1, -1, YELLOW);
	#endif
}

void	PmergeMe::_binaryInsert(std::vector<int>& main, std::vector<int>& pending, int depth)
{
	size_t	pendingNb;
	int		k;
	size_t	tk;
	int		toInsert;
	int		insertIndex;
	clock_t	start;
	clock_t	end;

	#if PRINTSTEP
	printcoldep(depth, IWHITE, "step 4a - Inserting using Jacobstahl sequence to ensure distribution");
	#else
	(void) depth;
	#endif
	start = clock();
	pendingNb = pending.size();
	k = 1;
	tk = _jacob[k];
	while (tk < pendingNb)
	{
		toInsert = pending[tk];
		insertIndex = _binarySearchIndex(main, toInsert, main.size() - 1);
		#if PRINTSTEP
		printcoldep(depth, IWHITE, tk);
		_printVec(main, "main", depth, insertIndex, insertIndex, GREEN);
		_printVec(pending, "pending", depth, tk, tk, GREY);
		#endif
		main.insert(main.begin() + insertIndex, toInsert);
		pending.erase(pending.begin() + tk);
		--pendingNb;
		++k;
		tk = _jacob[k];
	}
	#if PRINTSTEP
	printcoldep(depth, IWHITE, "step 4b - Inserting remaining elements of pending");
	#endif
	if (pendingNb > 0)
	{
		for (std::vector<int>::reverse_iterator it = pending.rbegin(); it != pending.rend(); ++it)
		{
			start = clock();
			int insertIndex = _binarySearchIndex(main, *it, main.size() - 1);
			main.insert(main.begin() + insertIndex, *it);
			end = clock();
			_timeToInsert += double(end - start) / double(CLOCKS_PER_SEC);
			#if PRINTSTEP
			_printVec(main, "main", depth, insertIndex, insertIndex, GREEN);
			_printVec(pending, "pending", depth, std::distance(pending.begin(), it.base()) -1, pending.size() - 1, GREY);
			#endif
		}
	}
	end = clock();
	_timeToInsert += double(end - start) / double(CLOCKS_PER_SEC);
}

void	PmergeMe::_sortVec(std::vector<int>& v, std::vector<int>* sorted, int depth)
{
	if (_checkEndOfRecursion(v, sorted, depth))
		return ;

	std::vector<std::pair<int, int> >	pairs;
	_sortPairs(v, pairs, depth);

	std::vector<int> main;
	_sortBiggest(v.size(), main, pairs, depth);
	
	std::vector<int> pending;
	_createPending(v, pending, pairs, depth);

	_binaryInsert(main, pending, depth);

	*sorted = main;
}

void	PmergeMe::initVec(int ac, char **av)
{
	int val;
	char *end;

	for (int i = 0; i < ac; i++)
	{
		val = static_cast<int>(std::strtol(av[i], &end, 10));
		_vec.push_back(val);
	}
}

/*************************************************************
*		    👁️‍ GETTERS and SETTERS				 			*
*************************************************************/
