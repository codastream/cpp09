#include "PmergeMe.hpp"
#include "debug.hpp"

PmergeMe::PmergeMe() : _jacob(generateJSequence(21)) {}

PmergeMe::PmergeMe(const PmergeMe& other) : _vec(other._vec) {}

PmergeMe::~PmergeMe() {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
	if (*this != other)
		_vec = other._vec;
	return (*this);
}

/***************************************************************
*					📁 CLASS ATTRIBUTES							*
***************************************************************/

static const int PmergeMe::_jacobLen = 21;
int PmergeMe::_nbCompMerge = 0;
int PmergeMe::_nbCompInsert = 0;

/***************************************************************
*					🛠️ FUNCTIONS								*
***************************************************************/

/// @brief compute the theoretical worst case scenario in terms of nb of comparisons
/// @param n number of elements to be sorted
/// @return aggregated number of comparisons
static int	computeMaxComparisons(size_t n)
{
	int sum = 0;
	for (int k = 1; k <= n; ++k)
	{
		double value = (3.0 / 4.0) * k;
		sum += static_cast<int>(std::ceil(std::log2(value)));
	}
	return sum;
}

static void	fillMain(t_vec& main, t_vec& data, size_t elemSize)
{
	size_t	mainSize = data.size() / elemSize;
	main.pushBack(elemSize - 1);
	for (size_t i = 1)
}

static std::vector<int>	generateJSequence()
{
	std::vector<int> v;
	v.push_back(0);
	v.push_back(1);
	for(int i = 2; i < _jacobLen; ++i)
	{
		v.push_back(v[i - 1] + 2 * v[i - 2]);
	}
	return v;
}

size_t PmergeMe::_computeBatchSize(size_t n)
{
	if (n == 0)
		return 0;
	else if (n >= _jacobLen)
		throw std::out_of_range("trying to reach beyond registered elems of Jacobstahl sequence");
	return (_jacob[n] - _jacob[n - 1]);
}

static t_vec::iterator	binarySearch(t_vec& main, int start, int end, int val)
{
	int	insertIdx = 0;
	while (start <= right)
	{
		int mid = start + (end - start) / 2;
		if (main[mid] > val)
		{
			right = mid - 1;
			insertIdx = mid;
		}
		else if (main[mid] < val)
		{
			left = mid + 1;
			insertIdx = mid + 1;
		}
	}
	return main.begin() + insertIdx;
}

static void	merge(t_vec& data, size_t elemSize)
{
	#ifdef DEBUG
	printByPair(BLUE "before merge:\t", data, elemSize);
	#endif

	#ifdef DEBUG
	printByPair(BLUE "after merge:\t", data, elemSize);
	#endif
}

static void	insert(t_vec& data, size_t elemSize)
{
	t_vec main;

	#ifdef DEBUG
	printData(BLUE "main:\t\t", , elemSize);
	#endif
}
