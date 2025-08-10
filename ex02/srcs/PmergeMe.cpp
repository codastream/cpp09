#include "PmergeMe.hpp"
#include "util.hpp"

/************************************************************
*				🥚 CONSTRUCTORS & DESTRUCTOR				*
************************************************************/

PmergeMe::PmergeMe(void) : _vec() {}

PmergeMe::PmergeMe(const PmergeMe& inst) : _vec(inst._vec) {}

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
	printcol(BLUE, "jacob seq");
	for (std::vector<int>::const_iterator cit = v.begin(); cit != v.end(); ++cit)
		std::cout << *cit << " ";
	std::cout << std::endl;
	return v;
}

void	PmergeMe::doSort()
{
	std::vector<int> sorted;
	sortVec(_vec, &sorted);
}

void	PmergeMe::sortVec(std::vector<int>& v, std::vector<int>* sorted)
{
	// stop conditions
	if (v.size() <= 1)
		return ;
	if (v.size() == 2)
	{
		if (v[0] < v[1])
			std::swap(v[0], v[1]);
		*sorted = v;
		return ;
	}

	// step 1 make pairs <max, min>
	bool canCompare = true;
	for (size_t i = 0; i < v.size(); ++i)
	{
		if (canCompare)
		{
			if (i < v.size() && v[i] < v[i + 1])
			{
				// std::cout << "swapping " << v[i] << " with " << v[i + 1] << std::endl;
				std::swap(v[i], v[i + 1]);
			}
		}
		canCompare = !canCompare;
	}

	// step 2 recursively sort max
	std::vector<int> vmax;
	bool canAdd = true;
	for (size_t i = 0; i < v.size(); ++i)
	{
		if (canAdd)
			vmax.push_back(v[i]);
		canAdd = !canAdd;
	};
	std::vector<int> vmaxSorted;
	sortVec(vmax, &vmaxSorted);
	
	// step 3 building main sorted
	std::vector<int> main;
	main.push_back(v[1]); // pushing min
	main.insert(main.end(), vmaxSorted.begin(), vmaxSorted.end());
	std::vector<int> pending;
	for (size_t i = 3; i < v.size(); i += 2)
	{	
		pending.push_back(v[i]);
	};
	if (v.size() % 2 == 1)
		pending.push_back(v.back());

	// step 4 insertion
	int pendingNB = pending.size();
	std::vector<int> jacob = generateJSequence(pendingNB);
	for (size_t i = 1; i < jacob.size() && jacob[i] < pendingNB; ++i)
	{
		int extractIndex = jacob[i];

		if (extractIndex < pendingNB)
		{
			int toInsert = pending[extractIndex]; // index of original pairs + getting smallest
			std::vector<int>::iterator it = std::lower_bound(main.begin(), main.end(), toInsert);
			main.insert(it, toInsert);
		}
		else
		{
			int toInsert = *pending.rbegin();
			std::vector<int>::iterator it = std::lower_bound(main.begin(), main.end(), toInsert);
			main.insert(it, toInsert);
			pending.pop_back();
		}
	}

	// check print vector
	printcol(BLUE, "sorted");
	for (std::vector<int>::const_iterator cit = main.begin(); cit != main.end(); ++cit)
		std::cout << *cit << std::endl;

	*sorted = main;
}

void	PmergeMe::initVec(int ac, char **av)
{
	int val;

	for (int i = 0; i < ac; i++)
	{
		val = std::atoi(av[i]);
		_vec.push_back(val);
	}
}

/*************************************************************
*		    👁️‍ GETTERS and SETTERS				 			*
*************************************************************/
