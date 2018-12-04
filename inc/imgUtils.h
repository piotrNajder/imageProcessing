#ifndef IMGUTILS_H
#define IMGUTILS_H

#include "cImage.h"
#include <string>

template <typename T1 = unsigned char, typename T2 = unsigned char>
void integral_image(T1 **inArray, T2 **outArray, uint32_t w, uint32_t h);

template <typename T1 = unsigned char, typename T2 = unsigned char>
void integral_image_sqr(T1 **inArray, T2 **outArray, uint32_t w, uint32_t h);

template <typename T = unsigned char>
uint64_t neighborsSum(T **in, uint32_t r, uint32_t c, uint32_t x_offset, uint32_t y_offset, uint32_t block_size, uint32_t& elems);

template <typename T = unsigned char>
uint64_t neighborsSum2(T **in, uint32_t r, uint32_t c, uint32_t x_offset, uint32_t y_offset, uint32_t block_size, uint32_t& elems);

template <typename T = unsigned char>
void copyWithPadding(T **inArray, T **outArray, uint32_t inRows, uint32_t inCols, uint32_t outRows, uint32_t outCols);

// Workaround for linker problems with template functions
#include "../src/imgUtils.cpp"

#endif