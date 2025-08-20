#include "PmergeMe.hpp"

static bool	checkArgs(int ac, char **av)
{
	long vall;
	int	val;
	char *end;

	if (ac < 2)
	{
		std::cerr << RED << "usage: ./RPN <(> operand1 operand2 operator <)> ..." << NC << std::endl;
		return false;
	}
	for (int i = 1; i < ac; i++)
	{
		vall = std::strtol(av[i], &end, 10);
		if (vall > std::numeric_limits<int>::max() || vall < 0 || *end != '\0')
			return false;
		val = vall;
	}
	return true;
}

/// @brief compute the theoretical min nb of comparisons, which is ceil(log2(n!))
/// @param n number of elements to be sorted
/// @return aggregated number of comparisons
static int	computeMinComparisons(size_t n)
{
	if (n <= 1)
		return 0;
	long double sumLog2 = 0.0L;
	for (size_t k = 2; k <= n; ++k)
	{
		sumLog2 += log2(static_cast<long double>(k));
	}
	return static_cast<int>(std::ceil(sumLog2));
}

static bool	isSortedAsc(t_vec& v)
{
	if (v.size() < 2)
		return true;
	for (t_vec::const_iterator it = v.begin() + 1; it != v.end(); ++it)
	{
		if (*it < *(it - 1))
			return false;
	}
	return true;
}

int	main(int ac, char** av)
{
	if (!checkArgs(ac, av))
	{
		puterr();
		return 1;
	}

	PmergeMe	miVec;
	int			n = --ac;

	printTitle(BG_BLUE, "RESULTS");
	printTitle(BLUE, "Input size (n)");
	std::cout << n << std::endl;

	printTitle(BG_CYAN, "vector");

	printTitle(CYAN, "Before");
	miVec.initVec(n, ++av);
	printData("data:\t", &(miVec.vec), 0, 1);

	miVec.sort(&(miVec.vec));

	#ifdef DEBUG
	if (!isSortedAsc(miVec.vec) || miVec.vec.size() != static_cast<unsigned long>(n))
	{
		std::cout << RED << "not sorted !" << NC << std::endl;
		std::cout << "input of size " << n << " and sorted of size " << miVec.vec.size() << std::endl;
	}
	else
		std::cout << GREEN << "sorted !" << NC << std::endl;
	#else
	(void) isSortedAsc;
	(void) computeMinComparisons;
	#endif

	printTitle(CYAN, "After");
	printData("sorted:\t", &(miVec.vec), 0, 1);

	#ifdef DEBUG
	printTitle(CYAN, "Expected theoretical complexity if n <= 12");
	std::cout << BLUE << "theoretical min\t" << NC << computeMinComparisons(n) << std::endl;

	printTitle(CYAN, "Nb of comparisons");
	std::cout << BLUE << "merge\t" << NC << miVec.nbCompMerge << std::endl;
	std::cout << BLUE << "insert\t" << NC << miVec.nbCompInsert << std::endl;
	std::cout << B_BLUE << "total\t" << NC << miVec.nbCompMerge + miVec.nbCompInsert << std::endl;
	#endif

	std::ostringstream oss;
	std::string msg;
	oss << "Time to process a range of " << n << " with std::vector";
	msg = oss.str();
	printTitleCompute(CYAN, msg);
	std::cout << BLUE << "merge\t" << NC << std::fixed << std::setprecision(5) << miVec.timeToMerge << " µsec" << std::endl;
	std::cout << BLUE << "insert\t" << NC << std::fixed << std::setprecision(5) << miVec.timeToInsert << " µsec" << std::endl;
	std::cout << B_BLUE << "total\t" << NC << std::fixed << std::setprecision(5) << miVec.timeToMerge + miVec.timeToInsert << " µsec" << std::endl;
}