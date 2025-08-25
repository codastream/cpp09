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

static int	computeFJWorstComparisons(size_t n)
{
	if (n < 1)
		return 0;
	int sum = 0.0L;
	for (size_t k = 1; k <= n; ++k)
	{
		double value = (3.0 / 4.0) * k;
		sum += static_cast<int>(std::ceil(log2(value)));
	}
	return sum;
}

void	checkVector(PmergeMe& mi, int n)
{
	printTitle(BG_CYAN, "vector");
	printTitle(CYAN, "Before");
	printData("data:\t", &(mi.vec), 0, 1);
	mi.sort(&(mi.vec));
	#ifdef DEBUG
	if (!isSortedAsc(mi.vec, 1) || mi.vec.size() != static_cast<unsigned long>(n) || !hasAllElemsOfFirst(mi.vecUnsorted, mi.vec))
	{
		std::cout << RED << "not sorted !" << NC << std::endl;
		std::cout << "input of size " << n << " and sorted of size " << mi.vec.size() << std::endl;
	}
	else
	{
		if (DEB)
			std::cout << GREEN << "sorted !" << NC << std::endl;
	}
	#else
	(void) isSortedAsc<std::vector<int> >;
	(void) computeMinComparisons;
	(void) computeFJWorstComparisons;
	#endif

	printTitle(CYAN, "After");
	printData("sorted:\t", &(mi.vec), 0, 1);

	#ifdef DEBUG
	printTitle(CYAN, "Expected theoretical complexity");
	std::cout << BLUE << "log2(n!)\t" << NC << computeMinComparisons(n) << std::endl;
	std::cout << BLUE << "log2(3n/4)\t" << NC << computeFJWorstComparisons(n) << std::endl;

	printTitle(CYAN, "Nb of comparisons");
	std::cout << BLUE << "merge\t\t" << NC << mi.nbCompMerge << std::endl;
	std::cout << BLUE << "insert\t\t" << NC << mi.nbCompInsert << std::endl;
	std::cout << B_BLUE << "total\t\t" << NC << mi.nbCompMerge + mi.nbCompInsert << std::endl;
	#endif

	std::ostringstream oss;
	std::string msg;
	oss << "Time to process a range of " << n << " with std::vector";
	msg = oss.str();
	printTitleCompute(CYAN, msg);
	std::cout << BLUE << "merge\t\t" << NC << std::fixed << std::setprecision(5) << mi.timeToMerge << " µsec" << std::endl;
	std::cout << BLUE << "insert\t\t" << NC << std::fixed << std::setprecision(5) << mi.timeToInsert << " µsec" << std::endl;
	std::cout << B_BLUE << "total\t\t" << NC << std::fixed << std::setprecision(5) << mi.timeToMerge + mi.timeToInsert << " µsec" << std::endl;
}

void	checkList(PmergeMe& mi, int n)
{
	printTitle(BG_CYAN, "list");
	printTitle(CYAN, "Before");
	printData("data:\t", &(mi.list), 0, 1);
	mi.sort(&(mi.list));
	#ifdef DEBUG
	if (!isSortedAsc(mi.list, 1) || mi.list.size() != static_cast<unsigned long>(n) || !hasAllElemsOfFirst(mi.listUnsorted, mi.list))
	{
		std::cout << RED << "not sorted !" << NC << std::endl;
		std::cout << "input of size " << n << " and sorted of size " << mi.list.size() << std::endl;
	}
	else
	{
		if (DEB)
			std::cout << GREEN << "sorted !" << NC << std::endl;
	}
	#else
	(void) isSortedAsc<std::list<int> >;
	(void) computeMinComparisons;
	(void) computeFJWorstComparisons;
	#endif

	printTitle(CYAN, "After");
	printData("sorted:\t", &(mi.list), 0, 1);

	#ifdef DEBUG
	printTitle(CYAN, "Nb of comparisons");
	std::cout << BLUE << "merge\t\t" << NC << mi.nbCompMerge << std::endl;
	std::cout << BLUE << "insert\t\t" << NC << mi.nbCompInsert << std::endl;
	std::cout << B_BLUE << "total\t\t" << NC << mi.nbCompMerge + mi.nbCompInsert << std::endl;
	#endif

	std::ostringstream oss;
	std::string msg;
	oss << "Time to process a range of " << n << " with std::list";
	msg = oss.str();
	printTitleCompute(CYAN, msg);
	std::cout << BLUE << "merge\t\t" << NC << std::fixed << std::setprecision(5) << mi.timeToMerge << " µsec" << std::endl;
	std::cout << BLUE << "insert\t\t" << NC << std::fixed << std::setprecision(5) << mi.timeToInsert << " µsec" << std::endl;
	std::cout << B_BLUE << "total\t\t" << NC << std::fixed << std::setprecision(5) << mi.timeToMerge + mi.timeToInsert << " µsec" << std::endl;
}

int	main(int ac, char** av)
{
	if (!checkArgs(ac, av))
	{
		puterr();
		return 1;
	}

	PmergeMe	miV;
	PmergeMe	miL;
	int			n = --ac;

	printTitle(BG_BLUE, "RESULTS");
	printTitle(BLUE, "Input size (n)");
	std::cout << n << std::endl;

	char **argcp = av;
	miV.initContainers(n, ++av);
	miL.initContainers(n, ++argcp);

	checkVector(miV, n);
	checkList(miL, n);

	#ifdef DEBUG
	printTitle(CYAN, "Expected theoretical complexity");
	std::cout << BLUE << "log2(n!)\t" << NC << computeMinComparisons(n) << std::endl;
	std::cout << BLUE << "log2(3n/4)\t" << NC << computeFJWorstComparisons(n) << std::endl;
	#endif
}