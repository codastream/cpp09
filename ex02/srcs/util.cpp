#include "util.hpp"

void	printnl(const std::string &s)
{
	std::cout << s << std::endl;
}

void	printcol(int depth, const std::string& col, const std::string& msg)
{
	for (int i = 0; i < depth * 2; i++)
		std::cout << " ";
	std::cout << col << msg << NC << std::endl;
}

void	puterr()
{
	std::cerr << "Error" << std::endl;
}