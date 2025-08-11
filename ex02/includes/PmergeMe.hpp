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
		std::vector<std::pair<int, int> >	_pairs;

		void								_presortVec(std::vector<int>& v);
		void								_sortVec(std::vector<int>& v, std::vector<int>* sorted, int depth);
};

#endif