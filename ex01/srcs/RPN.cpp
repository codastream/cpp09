#include "RPN.hpp"
#include "util.hpp"

/************************************************************
*				🥚 CONSTRUCTORS & DESTRUCTOR				*
************************************************************/

RPN::RPN(void) {}

RPN::RPN(const RPN& inst) : _args(inst._args) {}

RPN::~RPN(void) {}

/************************************************************
*				➕ OPERATORS									*
************************************************************/

RPN& RPN::operator=(const RPN& inst) 
{
	if (this != &inst)
	{
		_args = inst._args;
	}
	return (*this);
}

/*************************************************************
*		        🛠️ FUNCTIONS								*
*************************************************************/

int	add(int a, int b)
{
	return a + b;
}

int	sub(int a, int b)
{
	return a - b;
}

int	mult(int a, int b)
{
	return a * b;
}

int	divi(int a, int b)
{
	return a / b;
}

typedef int (*opf)(int, int);

const opf opfuncs[4] = {add, sub, mult, divi};
const char opsigns[4] = {'+', '-', '*', '/'};

static void doOp(int a, int b, char op, int* res)
{
	for (size_t i = 0; i < 4; i++)
	{
		if (opsigns[i] == op)
		{
			*res = opfuncs[i](a, b);
			break ;
		}
	}
}

bool	RPN::compute(const std::string& s)
{
	int	a;
	int b;
	int res = 0;

	const std::string& allowed = "+-*/";
	std::string::const_iterator cit = s.begin();
	for (; cit != s.end(); cit++)
	{
		if (cit != s.begin() && *cit == ' ')
		{
			if (*(--cit) == ' ')
			{
				puterr();
				return false;
			}
			else
				cit++;
		}
		else if (std::isdigit(*cit))
		{
			_args.push(*cit - '0');
		}
		else if (allowed.find(*cit) != std::string::npos)
		{
			if (_args.size() < 2)
			{
				puterr();
				return false;
			}
			b = _args.top();
			_args.pop();
			a = _args.top();
			_args.pop();
			doOp(a, b, *cit, &res);
			_args.push(res);
		}
		else
		{
			puterr();
			return false;
		}
	}
	if (cit != s.end() || _args.size() != 1)
	{
		puterr();
		return false;
	}
	else
		std::cout << res << std::endl;
	return true;
}

// void	RPN::_addArgs(int ac, char *av)
// {
// 	int i = std::strlen(av) - 1;
// 	while (av[i])
// 	{
// 		if (i > 0 && av[i - 1] && av[i - 1] != ' ')
// 		{
// 			std::string msg = "unexpected token " + av[i - 1];
// 			throw std::invalid_argument(msg);
// 		}
// 		_args.push(av[i]);
// 		i -= 2;
// 		ac--;
// 	}
// }

/*************************************************************
*		    👁️‍ GETTERS and SETTERS				 			*
*************************************************************/
