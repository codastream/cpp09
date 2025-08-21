#ifndef DEBUG_H
#define DEBUG_H

#include "utils.hpp"
#include "colors.hpp"

#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>

#define DEB 1

bool	isLastNbOfElem(size_t i, size_t elemSize);
bool	isFirstNbOfPair(size_t i, size_t elemSize);
bool	isLastNbOfPair(size_t i, size_t elemSize);
bool	isOdd(size_t i, size_t elemSize, size_t dataSize);
bool	isLastNbOfOdd(size_t i, size_t elemSize, size_t dataSize, size_t nbElems);
bool	isPartOfBatch(size_t i, size_t elemSize, size_t batchEndIdx, size_t batchSize);

void	printDepthOffset(int depth);

template <typename C>
bool	isSortedAsc(const C& cont, size_t elemSize);

template <typename C>
void	printByPairWithPending(const std::string& msgCol, const C* data, int depth, size_t elemSize, size_t batchSEndIdx, size_t batchSize, bool showMax = false);

template <typename C>
void	printByPair(const std::string& msgCol, const C* data, int depth, size_t elemSize, bool showMax = false);

template <typename C>
void	printData(const std::string& msgCol, const C* data, int depth, size_t elemSize, bool showMax = false);

#include "debug.tpp"

#endif