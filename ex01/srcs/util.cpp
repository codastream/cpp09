#include "util.hpp"

void	printnl(const std::string &s)
{
	std::cout << s << std::endl;
}

void	printcol(const std::string& col, const std::string& msg)
{
	std::cout << col << msg << NC << std::endl;
}

void	puterr(const std::string& s, const std::string& faulty)
{
	const std::string& msg = faulty.empty() ? s : s + " => " + faulty;
	std::cerr << RED << "Error: " << msg << NC << std::endl;
}