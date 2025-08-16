#ifndef DEBUG_H
#define DEBUG_H

#include "utils.hpp"
#include "colors.hpp"

#include <iostream>
#include <string>
#include <iomanip>

void	printDepthOffset(int depth);

template <typename C>
void	printByPair(const std::string& msgCol, const C& data, int depth, size_t elemSize);

template <typename C>
void	printData(const std::string& msgCol, const C& data, int depth, size_t elemSize);

#include "debug.tpp"

#endif