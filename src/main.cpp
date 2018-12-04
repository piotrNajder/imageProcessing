#include <string>
#include <chrono>
#include <iostream>
#include <math.h>
#include <cstring>
#include "../inc/cImage.h"
#include "../inc/imgUtils.h"

typedef std::chrono::high_resolution_clock::time_point tPoint;
typedef unsigned char pixel;
typedef uint32_t pixelUInt;

tPoint timeNow() { return std::chrono::high_resolution_clock::now(); }

std::string getExtensionFromPath(const std::string fPath);
std::string getFileNameFromPath(const std::string fPath);

void grayscaleImageBinarization(const std::string fName);
void rgbImageBinarization(const std::string fName);
void bradleyBinarization(const std::string fName, uint32_t bs = 7);
void sauvolaBinarizationIntegralImage(const std::string fName, uint32_t bs = 7);
void sauvolaBinarizationSimple(const std::string fName, uint32_t bs = 7);
void niblackBinarization(const std::string fName, uint32_t bs = 7);

bool integral_image_test(std::string testImagePath);
bool integral_image_sqr_test(std::string testImagePath);

int main(int argc, char *argv[]) {

	const std::string inF1 = "./input/book_grayscale_small.pgm";    // 904 x 427 x 8 BPP
	const std::string inF2 = "./input/book_grayscale.pgm";          // 2048 x 1536 x 8 BPP
	const std::string inF3 = "./input/book_grayscale_big.pgm";      // 4096 x 3072 x 8 BPP
	const std::string inF4 = "./input/book_rgb_small.ppm";          // 652 x 370 x 24 BPP
	const std::string inF5 = "./input/book_rgb.ppm";                // 2048 x 1536 x 24 BPP
	const std::string inF6 = "./input/book_rgb_big.ppm";            // 4096 x 3072 x 24 BPP
	const std::string inF7 = "./input/integral_test.pgm";			// Test image for integral methods

	sauvolaBinarizationIntegralImage(inF3, 13);
	sauvolaBinarizationSimple(inF3, 13);

	return 0;
}

void grayscaleImageBinarization(const std::string fName) {

	cImage<> inImg = cImage<>(fName);
	cImage<> outImg1 = cImage<>(1, inImg.rows, inImg.columns);
	cImage<> outImg2 = cImage<>(1, inImg.rows, inImg.columns);

	tPoint t1 = timeNow();

	for (uint32_t i = 0; i < inImg.rows; ++i) {
		for (uint32_t j = 0; j < inImg.columns; ++j) {
			outImg1.chG[i][j] = (inImg.chG[i][j] > 128) ? 255 : 0;
		}
	}

	tPoint t2 = timeNow();

	for (uint32_t i = 0; i < inImg.rows * inImg.columns; ++i) {
		*(*outImg2.chG + i) = (*(*inImg.chG + i) > 128) ? 255 : 0;
	}

	tPoint t3 = timeNow();	

	auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count();

	std::string fileExt = getExtensionFromPath(fName);
	std::string outFileName = getFileNameFromPath(fName);

	outImg1.write("./output/" + outFileName + "_gsBin1" + "." + fileExt);
	outImg2.write("./output/" + outFileName + "_gsBin2" + "." + fileExt);

	std::cout << outFileName + "." + fileExt + " method 1 binarization time: " << duration1 << std::endl;
	std::cout << outFileName + "." + fileExt + " method 2 binarization time: " << duration2 << std::endl;	
}

void rgbImageBinarization(const std::string fName) {

	cImage<> inImg = cImage<>(fName);
	cImage<> outImg = cImage<>(1, inImg.rows, inImg.columns);

	tPoint t1 = timeNow();

	const float Rcoef = 0.299;
	const float Gcoef = 0.587;
	const float Bcoef = 0.114;

	for (uint32_t i = 0; i < inImg.rows; ++i) {
		for (uint32_t j = 0; j < inImg.columns; ++j) {
			pixel gray_value = (pixel)((Rcoef * inImg.chR[i][j]) +
									   (Gcoef * inImg.chG[i][j]) +
									   (Bcoef * inImg.chB[i][j]));
			outImg.chG[i][j] = (gray_value > 128) ? 255 : 0;
		}
	}

	tPoint t2 = timeNow();

	std::string fileExt = getExtensionFromPath(fName);
	std::string outFileName = getFileNameFromPath(fName);

	outImg.write("./output/" + outFileName + "_rgbBin2" + ".pgm");

	auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

	std::cout << outFileName + "." + fileExt + " binarization time: " << duration1 << std::endl;	
}

void bradleyBinarization(const std::string fName, uint32_t bs) {

	cImage<> inImg = cImage<>(fName);
	cImage<> outImg = cImage<>(1, inImg.rows, inImg.columns);
	cImage<pixelUInt> intImg = cImage<pixelUInt>(1, inImg.rows, inImg.columns);

	tPoint t1 = timeNow();

	integral_image(inImg.chG, intImg.chG, inImg.columns, inImg.rows);
	
	double fact = 0.95 / ((2 * bs + 1) * (2 * bs + 1));

	for (uint32_t i = bs + 1; i < inImg.rows - bs - 1; ++i) {
		for (uint32_t j = bs + 1; j < inImg.columns - bs - 1; ++j) {
			if (inImg.chG[i][j] < (intImg.chG[i + bs][j + bs] +
								   intImg.chG[i - bs - 1][j - bs - 1] -
								   intImg.chG[i - bs - 1][j + bs] -
								   intImg.chG[i + bs][j - bs - 1]) * fact) {
				outImg.chG[i][j] = 0;
			}
			else {
				outImg.chG[i][j] = 255;
			}
		}
	}

	tPoint t2 = timeNow();

	std::string fileExt = getExtensionFromPath(fName);
	std::string outFileName = getFileNameFromPath(fName);

	outImg.write("./output/" + outFileName + "_bradleyBin" + "." + fileExt);

	auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

	std::cout << outFileName + "." + fileExt + " Bradley binarization time: " << duration1 << std::endl;
}

void sauvolaBinarizationIntegralImage(const std::string fName, uint32_t bs) {

	cImage<> inImg = cImage<>(fName);
	cImage<> inImgPadded = cImage<>(1, inImg.rows + 2 * bs, inImg.columns + 2 * bs);
	copyWithPadding(inImg.chG, inImgPadded.chG, inImg.rows, inImg.columns, inImgPadded.rows, inImgPadded.columns);

	cImage<> outImg = cImage<>(1, inImg.rows, inImg.columns);
	cImage<pixelUInt> intImg = cImage<pixelUInt>(1, inImgPadded.rows, inImgPadded.columns);
	cImage<pixelUInt> int_II_Img = cImage<pixelUInt>(1, inImgPadded.rows, inImgPadded.columns);

	tPoint t1 = timeNow();

	integral_image(inImgPadded.chG, intImg.chG, inImgPadded.columns, inImgPadded.rows);
	integral_image_sqr(inImgPadded.chG, int_II_Img.chG, inImgPadded.columns, inImgPadded.rows);

	int ncount = (2 * bs + 1) * (2 * bs + 1);	
	
	const float Rmax = 128.0;
	const float k = 0.12f;

	for (uint32_t i = 0, m = bs; m < inImgPadded.rows - bs; ++i, ++m) {
		for (uint32_t j = 0, l = bs; l < inImgPadded.columns - bs; ++j, ++l) {

			float sum = intImg.chG[m + bs][l + bs] +
						intImg.chG[m - bs][l - bs] -
						intImg.chG[m - bs][l + bs] -
						intImg.chG[m + bs][l - bs];
			float sum2 = int_II_Img.chG[m + bs][l + bs] +
						 int_II_Img.chG[m - bs][l - bs] -
						 int_II_Img.chG[m - bs][l + bs] -
						 int_II_Img.chG[m + bs][l - bs];

			float mean = sum / ncount;
			float std_dev = sqrt(sum2 / ncount - mean * mean);

			float T = (mean * (1.0 + k * (std_dev / Rmax - 1.0)));

			outImg.chG[i][j] = (inImg.chG[i][j] < T) ? 0 : 255;
		}
	}

	tPoint t2 = timeNow();

	std::string fileExt = getExtensionFromPath(fName);
	std::string outFileName = getFileNameFromPath(fName);

	outImg.write("./output/" + outFileName + "_SauvolaBin" + "." + fileExt);

	auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

	std::cout << outFileName + "." + fileExt + " Sauvola binarization time: " << duration1 << std::endl;
}

void sauvolaBinarizationSimple(const std::string fName, uint32_t bs) {
	cImage<> inImg = cImage<>(fName);
	cImage<> outImg = cImage<>(1, inImg.rows, inImg.columns);

	tPoint t1 = timeNow();
	
	uint32_t ncount = 0;
	const float Rmax = 128.0;
	const float k = 0.12f;

	for (uint32_t i = 0; i < inImg.rows; ++i) {
		for (uint32_t j = 0; j < inImg.columns; ++j) {
			ncount = 0;

			uint64_t sum = neighborsSum<>(inImg.chG, inImg.rows, inImg.columns, i, j, bs, ncount);
			float mean = sum / ncount;
			ncount = 0;

			uint64_t sum2 = neighborsSum2<>(inImg.chG, inImg.rows, inImg.columns, i, j, bs, ncount);
			float std_dev = sqrt(sum2 / ncount - mean * mean);			

			float T = (mean * (1.0 + k * (std_dev / Rmax - 1.0)));

			outImg.chG[i][j] = (inImg.chG[i][j] < T) ? 0 : 255;
		}
	}

	tPoint t2 = timeNow();

	std::string fileExt = getExtensionFromPath(fName);
	std::string outFileName = getFileNameFromPath(fName);

	outImg.write("./output/" + outFileName + "_SauvolaSimpleBin" + "." + fileExt);

	auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

	std::cout << outFileName + "." + fileExt + " Sauvola binarization simple time: " << duration1 << std::endl;
}

void niblackBinarization(const std::string fName, uint32_t bs) {

	cImage<> inImg = cImage<>(fName);
	cImage<> outImg = cImage<>(1, inImg.rows, inImg.columns);
	cImage<pixelUInt> intImg = cImage<pixelUInt>(1, inImg.rows, inImg.columns);
	cImage<pixelUInt> int_II_Img = cImage<pixelUInt>(1, inImg.rows, inImg.columns);

	tPoint t1 = timeNow();

	integral_image(inImg.chG, intImg.chG, inImg.columns, inImg.rows);
	integral_image_sqr(inImg.chG, int_II_Img.chG, inImg.columns, inImg.rows);

	int pixsInBlock = (2 * bs + 1) * (2 * bs + 1);

	for (uint32_t i = bs + 1; i < inImg.rows - bs; ++i) {
		for (uint32_t j = bs + 1; j < inImg.columns - bs; ++j) {

			float sum = intImg.chG[i + bs][j + bs] +
						intImg.chG[i - bs - 1][j - bs - 1] -
						intImg.chG[i - bs - 1][j + bs] -
						intImg.chG[i + bs][j - bs - 1];

			float sum2 = int_II_Img.chG[i + bs][j + bs] +
						 int_II_Img.chG[i - bs - 1][j - bs - 1] -
						 int_II_Img.chG[i - bs - 1][j + bs] -
						 int_II_Img.chG[i + bs][j - bs - 1];

			float mean = sum / pixsInBlock;
			float std_dev = sqrt(sum2 / pixsInBlock - mean * mean);

			float T = mean - 0.4 * std_dev;

			outImg.chG[i][j] = (inImg.chG[i][j] < T) ? 0 : 255;
		}
	}

	tPoint t2 = timeNow();

	std::string fileExt = getExtensionFromPath(fName);
	std::string outFileName = getFileNameFromPath(fName);

	outImg.write("./output/" + outFileName + "_NiblackBin" + "." + fileExt);

	auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

	std::cout << outFileName + "." + fileExt + " Niblack binarization time: " << duration1 << std::endl;
}

bool integral_image_test(std::string testImagePath) {
	cImage<> inImg = cImage<>(testImagePath);
	cImage<pixelUInt> intImg = cImage<pixelUInt>(1, inImg.rows, inImg.columns);

	tPoint t1 = timeNow();

	// Calculates the integral image 
	integral_image(inImg.chG, intImg.chG, inImg.columns, inImg.rows);	

	tPoint t2 = timeNow();	

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

	std::cout << " Integral image (" << inImg.rows << " x " << inImg.columns << ") calculated in: " << duration << " ms\n";

	// Reverse the integral image operation 
	// Process first column
	for (uint32_t y = intImg.rows - 1; y > 0; y--) {		
		intImg.chG[0][y] -= intImg.chG[0][y - 1];
	}	

	for (uint32_t x = 1; x < intImg.columns; x++) {
		for (uint32_t y = intImg.rows - 1; y > 0; y--) {	
			uint32_t row_sum = 0;
			for (uint32_t k = 0; k < x; k++) {
				row_sum += intImg.chG[k][y];
			}
			intImg.chG[x][y] -= intImg.chG[x][y - 1] + row_sum;			
		}
	}

	// Process first row
	for (uint32_t x = intImg.columns - 1; x > 0; x--) {
		intImg.chG[x][0] -= intImg.chG[x - 1][0];
	}

	// Check if inImg.chG == intImg.chG (values in arrays not arrays)
	for (uint32_t i = 0; i < inImg.rows; i++) {
		for (uint32_t j = 0; j < inImg.columns; j++) {
			if (inImg.chG[i][j] != intImg.chG[i][j]) {
				std::cout << "Error found in pixel at position: [" << i << ", " << j << "]";
				return false;
			}				
		}
	}
	std::cout << "Integral Image method checked: OK\n";
	return true;
}

bool integral_image_sqr_test(std::string testImagePath) {
	cImage<> inImg = cImage<>(testImagePath);
 	cImage<pixelUInt> intImg = cImage<pixelUInt>(1, inImg.rows, inImg.columns);

	tPoint t1 = timeNow();

	// Calculates the integral image 
	integral_image_sqr(inImg.chG, intImg.chG, inImg.columns, inImg.rows);	

	tPoint t2 = timeNow();	

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

	std::cout << " Integral image II (" << inImg.rows << " x " << inImg.columns << ") calculated in: " << duration << " ms\n";

	// Reverse the integral image operation 
	// Process first column
	for (uint32_t y = intImg.rows - 1; y > 0; y--) {		
		intImg.chG[0][y] = sqrt(intImg.chG[0][y] - intImg.chG[0][y - 1]);
	}	

	for (uint32_t x = 1; x < intImg.columns; x++) {
		for (uint32_t y = intImg.rows - 1; y > 0; y--) {	
			uint32_t row_sum = 0;
			for (uint32_t k = 0; k < x; k++) {
				row_sum += intImg.chG[k][y] * intImg.chG[k][y];
			}
			intImg.chG[x][y] = sqrt( intImg.chG[x][y] - intImg.chG[x][y - 1] - row_sum);			
		}
	}

	// Process first row
	for (uint32_t x = intImg.columns - 1; x > 0; x--) {
		intImg.chG[x][0] = sqrt(intImg.chG[x][0] - intImg.chG[x - 1][0]);
	}

	// Check if inImg.chG == intImg.chG (values in arrays not arrays)
	for (uint32_t i = 0; i < inImg.rows; i++) {
		for (uint32_t j = 0; j < inImg.columns; j++) {
			if (inImg.chG[i][j] != intImg.chG[i][j]) {
				std::cout << "Error found in pixel at position: [" << i << ", " << j << "]";
				return false;
			}				
		}
	}
	std::cout << "Integral Image II method checked: OK\n";
	return true;
}

std::string getExtensionFromPath(const std::string fPath) {
	return fPath.substr(fPath.find_last_of(".") + 1);
}

std::string getFileNameFromPath(const std::string fPath) {
	std::size_t dotPos = fPath.find_last_of(".");
	std::size_t slashPos = fPath.find_last_of("/");
	return fPath.substr(slashPos + 1, dotPos - slashPos - 1);
}