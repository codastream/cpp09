#include "PmergeMe.hpp"
#include "util.hpp"

/************************************************************
*				🥚 CONSTRUCTORS & DESTRUCTOR				*
************************************************************/

PmergeMe::PmergeMe(void) : _vec(), _pairs() {}

PmergeMe::PmergeMe(const PmergeMe& inst) : _vec(inst._vec), _pairs(inst._pairs) {}

PmergeMe::~PmergeMe(void) {}

/************************************************************
*				➕ OPERATORS									*
************************************************************/

PmergeMe& PmergeMe::operator=(const PmergeMe& inst) 
{
	if (this != &inst)
	{
		_vec = inst._vec;
		_pairs = inst._pairs;
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

static void	binaryInsert(std::vector<int>&v, int val, size_t maxPos)
{
	size_t searchEnd = std::min(maxPos, v.size());
	std::vector<int>::iterator start = v.begin();
	std::vector<int>::iterator end = v.begin() + searchEnd;
	std::vector<int>::iterator insertIt = std::lower_bound(start, end, val);
	std::cout << "inserting " << val << " between index 0 and " << searchEnd << "\n";
	v.insert(insertIt, val);
}

void	PmergeMe::doSort()
{
	std::vector<int> sorted;
	_presortVec(_vec);
	_sortVec(_vec, &sorted, 0);
	if (_vec.size() % 2 != 0)
	{
		int toInsert = _vec[_vec.size() - 1];
		binaryInsert(sorted, toInsert, sorted.size() - 1);
	}
}

static void printVec(std::vector<int>& v, const std::string& msg, int depth)
{
	for (int i = 0; i < depth * 2; i++)
		std::cout << " ";
	std::cout << BLUE << "depth " << depth << " " <<  msg << NC << std::endl;
	for (int i = 0; i < depth * 2; i++)
		std::cout << " ";
	for (std::vector<int>::const_iterator cit = v.begin(); cit != v.end(); ++cit)
		std::cout << *cit << " ";
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

void PmergeMe::_presortVec(std::vector<int>& v)
{
	// step 1 make pairs <max, min>
	for (size_t i = 0; i + 1 < v.size(); i += 2)
	{
		if (v[i] < v[i + 1])
		{
			std::swap(v[i], v[i + 1]);
		}
		_pairs.push_back(std::make_pair(v[i], v[i + 1]));
	}
}

void	PmergeMe::_sortVec(std::vector<int>& v, std::vector<int>* sorted, int depth)
{
	std::cout << YELLOW << " ===== " << depth << " ===== " << NC << std::endl;
	// stop conditions
	if (v.size() <= 1)
	{
		*sorted = v;
		printVec(v, "sorted", depth);
		return ;
	}
	if (v.size() == 2)
	{
		if (v[0] > v[1])
			std::swap(v[0], v[1]); // sorting in expected final asc order this time
		printVec(v, "sorted", depth);
		*sorted = v;
		return ;
	}

	// step 2 recursively sort max
	std::vector<int> vmax;
	vmax.reserve((v.size() + 1) / 2);
	for (size_t i = 0; i < v.size(); i += 2)
	{
		vmax.push_back(v[i]);
	};
	std::vector<int> vmaxSorted;
	_sortVec(vmax, &vmaxSorted, depth + 1);
	
	// step 3 building main sorted
	std::vector<int> main = vmaxSorted;
	int minfromPair0 = v[1];
	std::vector<int>::iterator it = std::lower_bound(main.begin(), main.end(), minfromPair0);
	main.insert(it, minfromPair0);
	printVec(main, "main", depth);

	std::vector<int> pending;
	pending.reserve(v.size() / 2);
	for (size_t i = 3; i < v.size(); i += 2)
	{	
		pending.push_back(v[i]);
	};
	if (v.size() % 2 == 1)
		pending.push_back(v.back());
	printVec(pending, "pending", depth);

	// step 4 insertion

	// taking index
	int pendingNb = static_cast<int>(pending.size());
	// std::vector<int> jacob = generateJSequence(pendingNb + 1);
	// printVec(jacob, "jacob", depth);

	// insertion
	int k = 1;
	size_t prevK = 1;
	size_t tk = computeJacobstahl(k);
	size_t batchSize = tk - prevK;
	size_t prevIdx = 0;
	int toInsert;
	while (tk < main.size())
	{
		for (int i = 0; i < depth * 2; i++)
			std::cout << " ";
		std::cout << "depth " << depth << " tk is " << tk << " - batchsize " << batchSize << " from " << prevIdx + batchSize << std::endl;
		for (size_t index = prevIdx + batchSize; index > prevIdx; index--)
		{
			toInsert = pending[index];
			binaryInsert(main, toInsert, tk);
			--pendingNb;
		}
		prevIdx = prevIdx + batchSize;
		prevK = tk;
		++k;
		tk = computeJacobstahl(k);
		printVec(pending, "pending", depth);
		printVec(main, "main", depth);
	}

	// check print vector
	printVec(main, "main sorted", depth);

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
