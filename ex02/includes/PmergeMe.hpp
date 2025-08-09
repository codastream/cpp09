#ifndef PMERGEME_H
#define PMERGEME_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <cstdlib>
#include <limits>

class PmergeMe {
	public:
		PmergeMe();
		virtual ~PmergeMe();
		PmergeMe(const PmergeMe& other);
		PmergeMe& operator=(const PmergeMe& other);

		void	sortList(std::list<int>& list);
		void	sortVec(std::list<int>& vec);

	private:
		std::vector<int>	_vec;
		std::list<int>		_list;
		bool				_checkArgs(int ac, char **av);
		void				_initList(int ac, char **av);
		void				_compareList(std::list<int>& list);
		void				_insert(std::list<int>& main, std::list<int>& from);
};

#endif