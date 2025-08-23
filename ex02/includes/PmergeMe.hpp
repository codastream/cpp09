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
#include <list>
#include <iterator>

#include <string>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <utility>

typedef std::vector<int> t_vec;
typedef std::list<int> t_list;

class PmergeMe {


	public:
		double					timeToMerge;
		double					timeToInsert;

		int						nbCompMerge;
		int						nbCompInsert;

		t_vec					vec;
		t_list					list;
		t_vec					vecUnsorted;
		t_list					listUnsorted;

		PmergeMe();
		~PmergeMe();
		PmergeMe(const PmergeMe& other);

		PmergeMe& 	operator=(const PmergeMe& other);

		void		initContainers(int ac, char **av);

		template <typename C>
		void		sort(C* data);
		
	private:

		static const int		_jacobLen;
		static std::vector<int>	_generateJSequence();

		t_vec					_jacob;

		size_t 					_computeBatchSize(size_t n);
		size_t					_binarySearch(t_vec& main, size_t start, size_t end, int val, size_t elemSize, bool isMainSize);
		size_t					_binarySearch(t_list& main, size_t start, size_t end, int val, size_t elemSize);
		
		int						_getValAtIndexInsert(t_list& l, size_t index);
		int						_getValAtIndexInsertOffset(t_list* l, size_t startIndex, int offset);

		template <typename T>
		bool					_isGreaterMerge(T lv, T rv);

		template <typename T>
		bool					_isLowerInsert(T lv, T rv);

		template <typename T>
		bool					_isEqualInsert(T lv, T rv);
		
		template <typename T>
		bool					_isUpperInsert(T lv, T rv);

		template <typename T>
		bool					_isBeyondLast(T startIdx, T endIdx);

		template <typename T>
		bool					_isOngoingBissect(T start, T end);

		template <typename C>
		void 					_sortRecursive(C* data, size_t elemSize, int depth);

		void					_merge(t_vec* data, size_t elemSize, int depth);
		void					_insert(t_vec* data, size_t elemSize, int depth);

		void					_merge(t_list* data, size_t elemSize, int depth);
		void					_insert(t_list* data, size_t elemSize, int depth);
};

#include "PmergeMe.tpp"

#endif