#ifndef PMERGEME_H
#define PMERGEME_H

#include "colors.hpp"
#include "utils.hpp"
#include "debug.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>

#include <algorithm>
#include <vector>
#include <iterator>

#include <string>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <utility>

typedef std::vector<int> t_vec;

class PmergeMe {


	public:
		double					timeToMerge;
		double					timeToInsert;

		int						nbCompMerge;
		int						nbCompInsert;

		t_vec					vec;

		PmergeMe();
		~PmergeMe();
		PmergeMe(const PmergeMe& other);

		PmergeMe& 	operator=(const PmergeMe& other);

		void		initVec(int ac, char **av);

		template <typename C>
		void		sort(C* data);
		
	private:

		static const int		_jacobLen;
		static std::vector<int>	_generateJSequence();


		t_vec					_jacob;

		size_t 					_computeBatchSize(size_t n);
		size_t					_binarySearch(t_vec& main, size_t start, size_t end, int val, size_t elemSize);
		
		template <typename T>
		bool					_isGreaterMerge(T lv, T rv);

		template <typename T>
		bool					_isGreaterInsert(T lv, T rv);
		
		template <typename T>
		bool					_isOngoingBissect(T start, T end);

		template <typename C>
		void 					_sortRecursive(C* data, size_t elemSize, int depth);

		void					_merge(t_vec* data, size_t elemSize, int depth);
		void					_insert(t_vec* data, size_t elemSize, int depth);
};

#include "PmergeMe.tpp"

#endif