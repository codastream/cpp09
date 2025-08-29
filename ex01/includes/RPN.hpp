#ifndef RPN_H
#define RPN_H

#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <cstring>
#include "util.hpp"

typedef int (*opf)(int, int);

class RPN {
	public:
		RPN();
		virtual ~RPN();
		RPN(const RPN& other);
		RPN& operator=(const RPN& other);

		bool	compute(const std::string& s);
		
	private:
		std::stack<int>	_args;
};

#endif