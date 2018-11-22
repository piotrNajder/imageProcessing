#ifdef IMGUTILS_H

#include "imgUtils.h"

template <typename T1, typename T2>
void integral_image(T1 **inArray, T2 **outArray, unsigned int w, unsigned int h)
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

template <typename T1 , typename T2>
void integral_image_sqr(T1 **inArray, T2 **outArray, unsigned int w, unsigned int h)
{
	unsigned int x, y;
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

void integral_image_test() {
	int rows = 4;
	int cols = 4;

	unsigned char **in = new unsigned char*[rows];
	in[0] = new unsigned char[rows*cols];
	for (int i = 1; i < rows; i++)
		in[i] = in[i - 1] + cols;

	unsigned char **out = new unsigned char*[rows];
	out[0] = new unsigned char[rows*cols];
	for (int i = 1; i < rows; i++)
		out[i] = out[i - 1] + cols;

	std::memset(in[0], 1, rows * cols);
	std::memset(out[0], 0, rows * cols);

	integral_image(in, out, cols, rows);

}

void integral_image_sqr_test() {

}

#endif