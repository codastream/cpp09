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
#include <cmath>
#include <iterator>
#include <utility>
#include <iomanip>

class PmergeMe {
	public:
		PmergeMe();
		virtual ~PmergeMe();
		PmergeMe(const PmergeMe& other);
		PmergeMe& operator=(const PmergeMe& other);

		void								doSort();
		void								initVec(int ac, char **av);
		
	private:
		std::vector<int>					_vec;
		int									_compMerge;
		int									_compInsert;

		void								_presortVec(std::vector<int>& v);
		void								_sortVec(std::vector<int>& v, std::vector<int>* sorted, int depth);
		int									_binarySearchIndex(std::vector<int>&v, int val, int tk);
};

#endif