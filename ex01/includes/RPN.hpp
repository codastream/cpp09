#ifndef RPN_H
#define RPN_H

#include <iostream>
#include <string>
#include <stack>
#include <cstring>

class RPN {
	public:
		RPN();
		virtual ~RPN();
		RPN(const RPN& other);
		RPN& operator=(const RPN& other);
		
	private:
		std::stack<char>	_args;
		void				_addArgs(int ac, char** av);
};

#endif