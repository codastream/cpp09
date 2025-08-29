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

	std::istringstream iss(s);
	std::string token;

	while (iss >> token)
	{
		char c = token[0];
		if (std::isdigit(c) && token.size() == 1)
		{
			_args.push(c - '0');
		}
		else if (allowed.find(c) != std::string::npos && token.size() == 1)
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
			if (c == '/' && b == 0)
			{
				puterr("can't divide by 0");
				return false;
			}
			doOp(a, b, c, &res);
			_args.push(res);
		}
		else
		{
			puterr();
			return false;
		}
	}
	
	if (_args.size() != 1)
	{
		puterr();
		return false;
	}
	else
		std::cout << res << std::endl;
	return true;
}
