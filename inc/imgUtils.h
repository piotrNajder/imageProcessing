#ifndef IMGUTILS_H
#define IMGUTILS_H

#include "cImage.h"

template <typename T1 = unsigned char, typename T2 = unsigned char>
void integral_image(T1 **inArray, T2 **outArray, unsigned int w, unsigned int h);

template <typename T1 = unsigned char, typename T2 = unsigned char>
void integral_image_sqr(T1 **inArray, T2 **outArray, unsigned int w, unsigned int h);

void integral_image_test();
void integral_image_sqr_test();

// Workaround for linker problems with template functions
#include "imgUtils.cpp"

#endif