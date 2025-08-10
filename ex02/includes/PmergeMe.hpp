#ifndef PMERGEME_H
#define PMERGEME_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <limits>
#include <cstdlib>
#include <algorithm>

class PmergeMe {
	public:
		PmergeMe();
		virtual ~PmergeMe();
		PmergeMe(const PmergeMe& other);
		PmergeMe& operator=(const PmergeMe& other);

		void				doSort();
		// std::vector<int>&	sortVec(std::vector<int>& v);
		void				sortVec(std::vector<int>& v, std::vector<int>* sorted);

		void				initVec(int ac, char **av);

	private:
		std::vector<int>	_vec;
};

#endif