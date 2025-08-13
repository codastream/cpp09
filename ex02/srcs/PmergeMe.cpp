#include "PmergeMe.hpp"
#include "util.hpp"

/************************************************************
*				🥚 CONSTRUCTORS & DESTRUCTOR				*
************************************************************/

PmergeMe::PmergeMe(void) : _vec(), _compMerge(0), _compInsert(0) {}

PmergeMe::PmergeMe(const PmergeMe& inst) : _vec(inst._vec), _compMerge(0), _compInsert(0) {}

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

std::vector<int>	generateJSequence(int size)
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
		return val > v[0] ? 1 : 0;
	while (start <= end)
	{
		_compInsert++;
		int mid = start + (end - start) / 2;
		if (v[mid] < val)
		{
			start = mid + 1;
		}
		else if (v[mid] > val)
		{
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
	std::vector<int> sorted;
	_sortVec(_vec, &sorted, 0);
}

static void _printVec(std::vector<int>& v, const std::string& msg, int depth, int outlineIdxStart, int outlineIdxend, const char* outlineColor)
{
	std::string color = YELLOW;
	if (!DEBUG)
		return ;
	for (int i = 0; i < depth; i++)
		std::cout << "  ";
	std::cout << BLUE << std::setw(16) << msg << NC;
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

static void _printPairs(std::vector<int>& v, std::vector<std::pair<int, int> >& pairs, int depth)
{
	size_t psize = pairs.size();
	for (int i = 0; i < depth * 2; i++)
		std::cout << " ";
	for (size_t i = 0 ; i < psize; ++i)
	{
		if (pairs[i].first != v[i * 2])
		{
			std::cout << std::left << YELLOW << ORANGE << std::left << std::setw(6) << pairs[i].first << std::setw(4) << pairs[i].second << YELLOW << "/ " << NC;
		}
		else
			std::cout  << std::left << YELLOW << std::left << std::setw(6) << pairs[i].first << std::setw(4) << pairs[i].second << YELLOW << "/ " << NC;
	}
	std::cout << std::endl;
}

static size_t computeJacobstahl(size_t k)
{
	size_t power = size_t(1) << (k + 1);
	size_t numerator;
	if ((k + 1) % 2 == 0)
		numerator = power - 1;
	else
		numerator = power + 1;
	return (numerator / 3);
}


void	PmergeMe::_sortVec(std::vector<int>& v, std::vector<int>* sorted, int depth)
{
	if (DEBUG)
		std::cout << YELLOW << "===== " << depth << " =====" << NC << std::endl;
	// stop conditions
	if (v.size() <= 1)
	{
		*sorted = v;
		_printVec(v, "sorted", depth, -1, -1, YELLOW);
		return ;
	}
	if (v.size() == 2)
	{
		if (v[0] > v[1])
		{
			std::swap(v[0], v[1]); // sorting in expected final asc order this time
			_printVec(v, "sorted", depth, 0, 1, ORANGE);
		}
		_compMerge++;
		*sorted = v;
		return ;
	}

	printcol(depth, BLUEBACK, "step 1 - making and sorting pairs");
	std::vector<std::pair<int, int> >	pairs;
	_printVec(v, "before presort", depth, -1, -1, YELLOW);
	std::vector<int> oldv = v;
	for (size_t i = 0; i + 1 < v.size(); i += 2)
	{
		if (v[i] < v[i + 1])
		{
			std::swap(v[i], v[i + 1]);
		}
		_compMerge++;
		pairs.push_back(std::make_pair(v[i], v[i + 1]));
	}
	_printPairs(oldv, pairs, depth);

	printcol(depth, BLUEBACK, "step 2 - recursively sorting the biggest elements");
	std::vector<int> vBig;
	vBig.reserve((v.size() + 1) / 2);
	for (size_t i = 0; i < pairs.size(); ++i)
	{
		vBig.push_back(pairs[i].first);
	};
	std::vector<int> vBigSorted;
	_sortVec(vBig, &vBigSorted, depth + 1);
	
	printcol(depth, BLUEBACK, "step 3 - building a final sorted main chain and a pending chain");
	std::vector<int> main = vBigSorted;
	int minfromPair0 = v[1];
	main.insert(main.begin(), minfromPair0);
	_printVec(main, "main", depth, 0, 0, GREEN);

	std::vector<int> pending;
	pending.reserve(v.size() / 2);
	for (size_t i = 1; i < pairs.size(); ++i)
	{
		pending.push_back(pairs[i].second);
	};
	if (v.size() % 2 == 1)
		pending.push_back(v.back());
	_printVec(pending, "pending", depth, -1, -1, YELLOW);

	printcol(depth, BLUEBACK, "step 4a - Inserting using Jacobstahl sequence to ensure distribution");
	size_t pendingNb = pending.size();
	int k = 1;
	int insertIndex = -1;
	size_t tk = computeJacobstahl(k);
	std::cout << "tk " << tk << std::endl;
	int toInsert;
	while (tk < pendingNb)
	{
		toInsert = pending[tk];
		insertIndex = _binarySearchIndex(main, toInsert, tk);
		if (insertIndex != -1)
			main.insert(main.begin() + insertIndex, toInsert);
		pending.erase(pending.begin() + tk);
		--pendingNb;
		++k;
		tk = computeJacobstahl(k);
		_printVec(pending, "pending", depth, tk, tk, RED);
		_printVec(main, "main", depth, insertIndex, insertIndex, GREEN);
	}
	printcol(depth, BLUEBACK, "step 4b - Inserting remaining elements of pending");
	if (pendingNb > 0)
	{
		for (std::vector<int>::reverse_iterator it = pending.rbegin(); it != pending.rend(); ++it)
		{
			int insertIndex = _binarySearchIndex(main, *it, pending.size());
			main.insert(main.begin() + insertIndex, *it);
			_printVec(pending, "pending", depth, std::distance(pending.begin(), it.base()) -1, pending.size() - 1, RED);
			_printVec(main, "main", depth, insertIndex, insertIndex, GREEN);
		}
	}

	_printVec(main, "main sorted", depth, -1, -1, YELLOW);
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
