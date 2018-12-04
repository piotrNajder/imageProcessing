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

template <typename T = unsigned char>
void copyWithPadding(T **inArray, T **outArray, uint32_t inRows, uint32_t inCols, uint32_t outRows, uint32_t outCols) {
	int32_t paddRows = outRows - inRows;
	int32_t paddCols = outCols - inCols;

	if (paddRows < 2 || paddCols < 2) {
		return;
	}

	// Ensure corners of new picture are set to 0
	memset(outArray[0], 0, outRows * outCols);

	int32_t rowsOffset = paddRows / 2;
	int32_t colsOffset = paddCols / 2;

	// Start with copy of orginal image into padded image
	for (int32_t r = 0; r < (int32_t)inRows; r++) {
		for (int32_t c = 0; c < (int32_t)inCols; c++)
			outArray[r + rowsOffset][c + colsOffset] = inArray[r][c];
	}

	//Fill the padded column - LEFT padding
	for (int32_t r = rowsOffset; r < (int32_t)(inRows + rowsOffset); r++) {
		for (int32_t c_out = colsOffset, c_in = inCols - 1;
			c_out >= 0; c_out--, c_in--) {
			outArray[r][c_out] = inArray[r - rowsOffset][c_in];
		}
	}
	//Fill the padded column - RIGHT padding
	for (int32_t r = rowsOffset; r < (int32_t)(inRows + rowsOffset); r++) {
		for (int32_t c_out = inCols, c_in = 0;
			c_out < (int32_t)outCols; c_out++, c_in++) {
			outArray[r][c_out] = inArray[r - rowsOffset][c_in];
		}
	}
	//Fill the padded rows - TOP padding
	for (int32_t r_out = rowsOffset - 1, r_in = inRows - 1;
		r_out < 0; r_out--, r_in--) {
		for (int32_t c = colsOffset; c < (int32_t)outCols - colsOffset; c++) {
			outArray[r_out][c] = inArray[r_in][c - colsOffset];
		}
	}
	//Fill the padded rows - BOTTOM padding
	for (int32_t r_out = outRows - rowsOffset, r_in = 0;
		r_out < (int32_t)outRows; r_out++, r_in++) {
		for (int32_t c = colsOffset; c < (int32_t)(outCols - colsOffset); c++) {
			outArray[r_out][c] = inArray[r_in][c - colsOffset];
		}
	}
}

#endif