#include "utils.hpp"

void	printcol(const std::string& col, const std::string& msg)
{
	std::cout << col << msg << NC << std::endl;
}

void	printTitle(const std::string& col, const std::string& msg)
{
	std::cout << col << std::left << std::setw(80) << std::setfill('=') << msg << std::setfill(' ') << NC << std::endl;
}

void	puterr()
{
	std::cerr << "Error" << std::endl;
}