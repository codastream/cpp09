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
/// 	which with Stirling's approximation is around n log2 n − 1.44 n + O(log n)
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

	printTitle(CYAN, "Before sort");
	miVec.initVec(n, ++av);
	printData("unsorted:\t", &(miVec.vec), 0, 1);

	miVec.sort(&(miVec.vec));

	if (!isSortedAsc(miVec.vec))
		std::cout << RED << "not sorted !" << NC << std::endl;
	else
		std::cout << GREEN << "sorted !" << NC << std::endl;


	printTitle(CYAN, "After sort");
	printData("sorted:\t\t", &(miVec.vec), 0, 1);

	printTitle(CYAN, "Expected complexity");
	std::cout << BLUE << "theoretical min\t" << NC << computeMinComparisons(n) << std::endl;

	printTitle(CYAN, "Nb of comparisons");
	std::cout << BLUE << "merge\t" << NC << miVec.nbCompMerge << std::endl;
	std::cout << BLUE << "insert\t" << NC << miVec.nbCompInsert << std::endl;
	std::cout << B_BLUE << "total\t" << NC << miVec.nbCompMerge + miVec.nbCompInsert << std::endl;

	printTitle(CYAN, "Compute time");
	std::cout << BLUE << "merge\t" << NC << std::fixed << std::setprecision(5) << miVec.timeToMerge << " µsec" << std::endl;
	std::cout << BLUE << "insert\t" << NC << std::fixed << std::setprecision(5) << miVec.timeToInsert << " µsec" << std::endl;
	std::cout << B_BLUE << "total\t" << NC << std::fixed << std::setprecision(5) << miVec.timeToMerge + miVec.timeToInsert << " µsec" << std::endl;
}