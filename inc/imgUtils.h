#ifndef IMGUTILS_H
#define IMGUTILS_H

#include "cImage.h"
#include <string>

template <typename T1 = unsigned char, typename T2 = unsigned char>
void integral_image(T1 **inArray, T2 **outArray, unsigned int w, unsigned int h);

template <typename T1 = unsigned char, typename T2 = unsigned char>
void integral_image_sqr(T1 **inArray, T2 **outArray, unsigned int w, unsigned int h);

template <typename T = unsigned char>
uint64_t neighborsSum(T **in, unsigned int r, unsigned int c, unsigned int x_offset, unsigned int y_offset,
					  unsigned int block_size, unsigned int& elems);

template <typename T = unsigned char>
uint64_t neighborsSum2(T **in, unsigned int r, unsigned int c, unsigned int x_offset, unsigned int y_offset,
					   unsigned int block_size, unsigned int& elems);

void integral_image_test();
void integral_image_sqr_test();

// Workaround for linker problems with template functions
#include "imgUtils.cpp"

#endif