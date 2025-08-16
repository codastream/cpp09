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
		PmergeMe();
		~PmergeMe();
		PmergeMe(const PmergeMe& other);

		PmergeMe& operator=(const PmergeMe& other);



		template <typename C>
		static void sort(C& data);
		
	private:
		t_vec				_vec;
		t_vec				_jacob;
		double				_timeToMerge;
		double				_timeToInsert;

		static int			_nbCompMerge;
		static int			_nbCompInsert;

		static const int	_jacobLen;

		size_t 				_computeBatchSize(size_t n);
		
		template <typename T>
		bool				_isFirstLowerMerge(T lv, T rv);

		template <typename T>
		bool				_isFirstLowerInsert(T lv, T rv);
		
		template <typename C>
		static void 		_sortRecursive(C& data, size_t elemSize);

		static void			_merge(t_vec& data, size_t elemSize);
		static void			_insert(t_vec& data, size_t elemSize);
};

#include "PmergeMe.tpp"

#endif