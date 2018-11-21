#include "../inc/imgUtils.h"

void integral_image(pixelArray inArray, pixelArray outArray, unsigned int w, unsigned int h)
{
	unsigned int pixelsSum = 0;
	for (unsigned int x = 0; x < w; ++x) {
		pixelsSum += inArray[0][x];
		outArray[0][x] = pixelsSum;
	}

	for (unsigned int y = 1; y < h; ++y) {
		pixelsSum = 0;
		for (unsigned int x = 0; x < w; ++x) {
			pixelsSum += inArray[y][x];
			outArray[y][x] = outArray[y-1][x] + pixelsSum;
		}
	}
}

void integral_image_sqr(pixelArray inArray, pixelArray outArray, unsigned int w, unsigned int h)
{
	unsigned int x,y;
	uint64_t suma;

	for (y = 0; y < h; ++y) {
		suma = 0;
		for (x = 0; x < w; ++x) {
			suma += (inArray[y][x] * inArray[y][x]);
			if (y == 0) outArray[y][x] = suma;
            else outArray[y][x] = outArray[y-1][x] + suma;
		}
	}
}