#ifdef IMGUTILS_H
#include "../inc/imgUtils.h"

template <typename T1, typename T2>
void integral_image(T1 **inArray, T2 **outArray, uint32_t w, uint32_t h) {
	uint32_t pixelsSum = 0;
	for (uint32_t x = 0; x < w; ++x) {
		pixelsSum += inArray[0][x];
		outArray[0][x] = pixelsSum;
	}

	for (uint32_t y = 1; y < h; ++y) {
		pixelsSum = 0;
		for (uint32_t x = 0; x < w; ++x) {
			pixelsSum += inArray[y][x];
			outArray[y][x] = outArray[y - 1][x] + pixelsSum;
		}
	}
}

template <typename T1, typename T2>
void integral_image_sqr(T1 **inArray, T2 **outArray, uint32_t w, uint32_t h) {
	
	uint64_t sum = 0;

	for (uint32_t y = 0; y < h; ++y) {
		sum = 0;
		for (uint32_t x = 0; x < w; ++x) {
			sum += (inArray[y][x] * inArray[y][x]);
			if (y == 0) outArray[y][x] = sum;
			else outArray[y][x] = outArray[y - 1][x] + sum;
		}
	}
}

template <typename T = unsigned char>
uint64_t neighborsSum(T **in, uint32_t r, uint32_t c, uint32_t x_offset, uint32_t y_offset,
	uint32_t block_size, uint32_t& elems) {
	uint64_t sum = 0;
	// Boundry check
	// if block is out of pixel array we resize the box to fit into array
	uint32_t start_posX = 0, start_posY = 0;
	uint32_t end_posX = r - 1, end_posY = c - 1;
	if (int64_t(int64_t(x_offset) - int64_t(block_size)) > start_posX) start_posX = x_offset - block_size;
	if (int64_t(int64_t(y_offset) - int64_t(block_size)) > start_posY) start_posY = y_offset - block_size;
	if (int64_t(int64_t(x_offset) + int64_t(block_size)) < end_posX) end_posX = x_offset + block_size;
	if (int64_t(int64_t(y_offset) + int64_t(block_size)) < end_posY) end_posY = y_offset + block_size;

	for (uint32_t i = start_posX; i < end_posX; i++) {
		for (uint32_t j = start_posY; j < end_posY; j++) {
			elems++;
			sum += in[i][j];
		}
	}
	return sum;
}

template <typename T = unsigned char>
uint64_t neighborsSum2(T **in, uint32_t r, uint32_t c, uint32_t x_offset, uint32_t y_offset,
	uint32_t block_size, uint32_t& elems) {
	uint64_t sum = 0;
	// Boundry check
	// if block is out of pixel array we resize the box to fit into array
	uint32_t start_posX = 0, start_posY = 0;
	uint32_t end_posX = r - 1, end_posY = c - 1;
	if (int64_t(int64_t(x_offset) - int64_t(block_size)) > start_posX)
		start_posX = x_offset - block_size;
	if (int64_t(int64_t(y_offset) - int64_t(block_size)) > start_posY)
		start_posY = y_offset - block_size;
	if (int64_t(int64_t(x_offset) + int64_t(block_size)) < end_posX)
		end_posX = x_offset + block_size;
	if (int64_t(int64_t(y_offset) + int64_t(block_size)) < end_posY)
		end_posY = y_offset + block_size;

	for (uint32_t i = start_posX; i < end_posX; i++) {
		for (uint32_t j = start_posY; j < end_posY; j++) {
			elems++;
			sum += in[i][j] * in[i][j];
		}
	}
	return sum;
}

template <typename T1 = unsigned char, typename T2 = unsigned char>
void copy_and_fill(T1 **inArray, T2 **outArray, uint32_t c_in, uint32_t r_in, uint32_t c_out, uint32_t r_out) {
	
	uint32_t cols_diff = c_out - c_in;
	uint32_t rows_diff = r_out - r_in;

	if (cols_diff < 2 || cols_diff % 2 != 0 ||
	    rows_diff < 2 || rows_diff % 2 != 0 ) {
			return;
		}

	// Copy input image in the middle of output image
	uint32_t c_offset = cols_diff / 2;
	uint32_t r_offset = rows_diff / 2;
	for (uint32_t i = 0; i < c_in; i++) {
		for (uint32_t j = 0; j < r_in; j++) {
			outArray[i + r_offset][j + c_offset] = inArray[i][j];
		}		
	}
}

#endif