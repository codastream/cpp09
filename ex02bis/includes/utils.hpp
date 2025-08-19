#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include "colors.hpp"

void	printColor(const std::string& col, const std::string& msg);
void	printTitle(const std::string& col, const std::string& msg);
void	printTitleCompute(const std::string& col, const std::string& msg);
void	puterr();

#endif