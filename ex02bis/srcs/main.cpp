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

/// @brief compute the theoretical worst case scenario in terms of nb of comparisons
/// @param n number of elements to be sorted
/// @return aggregated number of comparisons
static int	computeMaxComparisons(size_t n)
{
	int sum = 0;
	for (size_t k = 1; k <= n; ++k)
	{
		double value = (3.0 / 4.0) * k;
		sum += static_cast<int>(std::ceil(log2(value)));
	}
	return sum;
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
	std::cout << BLUE << "theoretical worst case = O (n log2(n) + sum(3/4n))\t" << NC << computeMaxComparisons(n) << std::endl;

	printTitle(CYAN, "Nb of comparisons");
	std::cout << BLUE << "merge\t" << NC << miVec.nbCompMerge << std::endl;
	std::cout << BLUE << "insert\t" << NC << miVec.nbCompInsert << std::endl;
	std::cout << B_BLUE << "total\t" << NC << miVec.nbCompMerge + miVec.nbCompInsert << std::endl;

	printTitle(CYAN, "Compute time");
	std::cout << BLUE << "merge\t" << NC << std::fixed << std::setprecision(5) << miVec.timeToMerge << " µsec" << std::endl;
	std::cout << BLUE << "insert\t" << NC << std::fixed << std::setprecision(5) << miVec.timeToInsert << " µsec" << std::endl;
	std::cout << B_BLUE << "total\t" << NC << std::fixed << std::setprecision(5) << miVec.timeToMerge + miVec.timeToInsert << " µsec" << std::endl;
}