#include "util.hpp"

void	printnl(const std::string &s)
{
	std::cout << s << std::endl;
}

void	printcol(const std::string& col, const std::string& msg)
{
	std::cout << col << msg << NC << std::endl;
}

void	puterr()
{
	std::cerr << "Error" << std::endl;
}

void	puterr(const std::string& msg)
{
	std::cerr << RED << "Error: " << NC << msg << std::endl;
}