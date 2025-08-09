#include "RPN.hpp"
#include "util.hpp"

/************************************************************
*				🥚 CONSTRUCTORS & DESTRUCTOR				*
************************************************************/

RPN::RPN(void) {}

RPN::RPN(const RPN& inst) {}

RPN::~RPN(void) {}

/************************************************************
*				➕ OPERATORS									*
************************************************************/

RPN& RPN::operator=(const RPN& inst) 
{
	if (this != &inst)
	{
	}
	return (*this);
}

/*************************************************************
*		        🛠️ FUNCTIONS								*
*************************************************************/

void	RPN::_addArgs(int ac, char *av)
{
	int i = std::strlen(av) - 1;
	while (av[i])
	{
		if (av[i - 1] && av[i - 1] != ' ')
		{
			std::str msg = "unexpected token " + av[i - 1];
			throw std::invalid_argument(msg);
		}
		_args.push(av[i])
		i -= 2;
		ac--;
	}
}

/*************************************************************
*		    👁️‍ GETTERS and SETTERS				 			*
*************************************************************/
