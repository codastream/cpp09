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
		double								_timeToMerge;
		double								_timeToInsert;
		std::vector<int>					_jacob;

		void								_presortVec(std::vector<int>& v);
		void								_sortVec(std::vector<int>& v, std::vector<int>* sorted, int depth);
		int									_binarySearchIndex(std::vector<int>&v, int val, int tk);
		bool								_checkEndOfRecursion(std::vector<int>& v, std::vector<int>* sorted, int depth);
		void								_sortPairs(std::vector<int>&v, std::vector<std::pair<int, int> >&	pairs, int depth);
		void								_sortBiggest(size_t vSize, std::vector<int>& main, std::vector<std::pair<int, int> >&	pairs, int depth);
		void								_createPending(std::vector<int>& v, std::vector<int>& pending, std::vector<std::pair<int, int> >&	pairs, int depth);
		void								_binaryInsert(std::vector<int>& main, std::vector<int>& pending, int depth);
};

#endif