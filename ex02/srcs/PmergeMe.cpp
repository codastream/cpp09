#include "PmergeMe.hpp"
#include "util.hpp"

/************************************************************
*				🥚 CONSTRUCTORS & DESTRUCTOR				*
************************************************************/

PmergeMe::PmergeMe(void) {}

PmergeMe::PmergeMe(const PmergeMe& inst) : _bec(inst._vec), _list(inst._list) {}

PmergeMe::~PmergeMe(void) {}

/************************************************************
*				➕ OPERATORS									*
************************************************************/

PmergeMe& PmergeMe::operator=(const PmergeMe& inst) 
{
	if (this != &inst)
	{
		_vec = inst._vec;
		_list = inst._list;
	}
	return (*this);
}

/*************************************************************
*		        🛠️ FUNCTIONS								*
*************************************************************/

bool	PmergeMe::_checkArgs(int ac, char **av)
{
	long vall;
	int	val;

	for (int i = 1; i < ac; i++)
	{
		vall = std::strtol(av[i]);
		if (vall > std::numeric<int>::max() || vall < 0)
			return false;
		val = vall;
	}
}

void	PmergeMe::_initList(int ac, char **av)
{
	
}

void	PmergeMe::_compareList(std::list<int>& list)
{

}

void	PmergeMe::_insert(std::list<int>& main, std::list<int>& from)
{

}

/*************************************************************
*		    👁️‍ GETTERS and SETTERS				 			*
*************************************************************/
