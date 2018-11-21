#ifndef IMGUTILS_H
#define IMGUTILS_H

#include "../inc/cImage.h"

void integral_image(pixelArray inArray, pixelArray outArray, unsigned int w, unsigned int h);
void integral_image_sqr(pixelArray inArray, pixelArray outArray, unsigned int w, unsigned int h);

#endif