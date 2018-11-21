#include <string>
#include <chrono>
#include <iostream>
#include "../inc/cImage.h"
#include "../inc/imgUtils.h"

typedef std::chrono::high_resolution_clock::time_point tPoint;

tPoint timeNow() {return std::chrono::high_resolution_clock::now();}

void grayscaleImageBinarization(const std::string fName);
void rgbImageBinarization(const std::string fName);
void bradleyBinarization(const std::string fName);          // Implemented to work only with grayscale images
void sauvolaBinarization(const std::string fName);          // Implemented to work only with grayscale images
void niblackBinarization(const std::string fName);          // Implemented to work only with grayscale images

int main (int argc, char *argv[]) {

    const std::string inF1 = "./input/book_grayscale_small.pgm";    // 904 x 427 x 8 BPP
    const std::string inF2 = "./input/book_grayscale.pgm";          // 2048 x 1536 x 8 BPP
    const std::string inF3 = "./input/book_grayscale_big.pgm";      // 4096 x 3072 x 8 BPP
    const std::string inF4 = "./input/book_rgb_small.pgm";          // 652 x 370 x 24 BPP
    const std::string inF5 = "./input/book_rgb.pgm";                // 2048 x 1536 x 24 BPP
    const std::string inF6 = "./input/book_rgb_big.pgm";            // 4096 x 3072 x 24 BPP

    grayscaleImageBinarization(inF1);
    grayscaleImageBinarization(inF2);
    grayscaleImageBinarization(inF2);

    rgbImageBinarization(inF4);
    rgbImageBinarization(inF5);
    rgbImageBinarization(inF6);

    bradleyBinarization(inF1);
    bradleyBinarization(inF2);
    bradleyBinarization(inF2);

    sauvolaBinarization(inF1);
    sauvolaBinarization(inF2);
    sauvolaBinarization(inF2);

    niblackBinarization(inF1);
    niblackBinarization(inF2);
    niblackBinarization(inF2);
    
    return 0;
}

void grayscaleImageBinarization(const std::string fName) {

    cImage inImg = cImage(fName);    
    std::size_t dotPos = fName.find_last_of(".");
    std::size_t slashPos = fName.find_last_of("/");

    std::string fileExt = fName.substr(dotPos + 1);
    std::string outFileName = fName.substr(slashPos + 1, dotPos);

    cImage outImg = cImage(1, inImg.rows, inImg.columns);

    tPoint t1 = timeNow();
    
    for (unsigned int i = 0; i < inImg.rows; ++i) {
		for (unsigned int j = 0; j < inImg.columns; ++j){            
		    outImg.chG[i][j] = ( inImg.chG[i][j] > 128 ) ? 255 : 0;
		}
	}

    tPoint t2 = timeNow();

    outImg.write("./output/" + outFileName + "_gsBin1" + "." + fileExt);

    tPoint t3 = timeNow();

	for (unsigned int i = 0; i < inImg.rows * inImg.columns; ++i) {
		*(*outImg.chG + i) = (*(*inImg.chG + i) > 128 ) ?  255 : 0;
	}
    
    tPoint t4 = timeNow();

    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>( t4 - t3 ).count();

    std::cout << outFileName + "." + fileExt + " method 1 binarization time: " << duration1 << std::endl;
    std::cout << outFileName + "." + fileExt + " method 2 binarization time: " << duration2 << std::endl;

    outImg.write("./output/" + outFileName + "_gsBin2" + "." + fileExt);
}

void rgbImageBinarization(const std::string fName) {
    cImage inImg = cImage(fName);    
    std::size_t dotPos = fName.find_last_of(".");
    std::size_t slashPos = fName.find_last_of("/");

    std::string fileExt = fName.substr(dotPos + 1);
    std::string outFileName = fName.substr(slashPos + 1, dotPos);

    cImage outImg = cImage(1, inImg.rows, inImg.columns);

    tPoint t1 = timeNow();
    
    for ( int i = 0; i < inImg.rows ; ++i) {
		for (int j = 0; j < inImg.columns; ++j) {
			unsigned gray_value = unsigned char ((0.299 * inImg.chR[i][j]) + 
                                                 (0.587 * inImg.chG[i][j]) + 
                                                 (0.114 * inImg.chB[i][j]));
            outImg.chG[i][j] = (gray_value > 128 ) ?  255 : 0;
		}
	}

    tPoint t2 = timeNow();

    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();

    std::cout << outFileName + "." + fileExt + " binarization time: " << duration1 << std::endl;

    outImg.write("./output/" + outFileName + "_rgbBin2" + "." + fileExt);
}

void bradleyBinarization(const std::string fName) {

    cImage inImg = cImage(fName);    
    std::size_t dotPos = fName.find_last_of(".");
    std::size_t slashPos = fName.find_last_of("/");

    std::string fileExt = fName.substr(dotPos + 1);
    std::string outFileName = fName.substr(slashPos + 1, dotPos);

    cImage outImg = cImage(1, inImg.rows, inImg.columns);
    cImage intImg = cImage(1, inImg.rows, inImg.columns);

    tPoint t1 = timeNow();

    integral_image(inImg.chG, intImg.chG, inImg.rows, inImg.columns);

	int n = 7;
	double fact = 0.95 / ((2 * n + 1) * (2 * n + 1));
	
	for (int i = n + 1; i < inImg.rows - n - 1; ++i) {
		for (int j = n + 1; j < inImg.columns - n - 1; ++j) {			
			if (inImg.chG[i][j] < ( intImg.chG[i + n][j + n] +
                                    intImg.chG[i - n - 1][j-n-1] -
                                    intImg.chG[i - n - 1][j + n] -
                                    intImg.chG[i + n][j - n - 1] ) * fact) {
                outImg.chG[i][j] = 0;
            }				
			else {
                outImg.chG[i][j] = 255;
            }
		}
	}

    tPoint t2 = timeNow();

    outImg.write("./output/" + outFileName + "_bradleyBin" + "." + fileExt);    

    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();

    std::cout << outFileName + "." + fileExt + " Bradley binarization time: " << duration1 << std::endl;    
}

void sauvolaBinarization(const std::string fName) {
    cImage inImg = cImage(fName);    
    std::size_t dotPos = fName.find_last_of(".");
    std::size_t slashPos = fName.find_last_of("/");

    std::string fileExt = fName.substr(dotPos + 1);
    std::string outFileName = fName.substr(slashPos + 1, dotPos);

    cImage outImg = cImage(1, inImg.rows, inImg.columns);
    cImage intImg = cImage(1, inImg.rows, inImg.columns);
    cImage int_II_Img = cImage(1, inImg.rows, inImg.columns);
	
	tPoint t1 = timeNow();
	
	integral_image(inImg.chG, intImg.chG, inImg.rows, inImg.columns);
	integral_image_sqr(inImg.chG, int_II_Img.chG, inImg.rows, inImg.columns);

	int n = 7;
	int ncount = (2 * n + 1) * (2 * n + 1);
	
	float sum = 0;	//suma jasnosci
	float sum2 = 0;	//suma kwadratow jasnosci
	float m = 0.0;	//srednia jasnosc
	float s = 0.0;	//odchylenie standardowe
	float T = 0.0;
	float Rmax = 128.0;
	float k = 0.12f;
	
	for (unsigned int i = n + 1; i < inImg.rows - n; ++i) {
		for (unsigned int j = n +1; j < inImg.columns - n; ++j) {

			sum = intImg.chG[i + n][j + n] +
                  intImg.chG[i - n - 1][j - n - 1] -
                  intImg.chG[i - n - 1][j + n] -
                  intImg.chG[i + n][j - n - 1];
			sum2 = int_II_Img.chG[i + n][j + n] + 
                   int_II_Img.chG[i - n - 1][j - n - 1] -
                   int_II_Img.chG[i - n - 1][j + n] - 
                   int_II_Img.chG[i + n][j - n - 1];

			m = sum / ncount;
			s = sqrt(sum2 / ncount - m * m);
			
			T = ( m * ( 1.0 + k * (s / Rmax - 1.0 ) ) );    //Sauvola method

			outImg.chG[i][j] = (inImg.chG[i][j] < T ) ?  0 : 255;
		}
	}

    tPoint t2 = timeNow();

    outImg.write("./output/" + outFileName + "_SauvolaBin" + "." + fileExt);    

    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();

    std::cout << outFileName + "." + fileExt + " Sauvola binarization time: " << duration1 << std::endl;
}

void niblackBinarization(const std::string fName) {
    cImage inImg = cImage(fName);    
    std::size_t dotPos = fName.find_last_of(".");
    std::size_t slashPos = fName.find_last_of("/");

    std::string fileExt = fName.substr(dotPos + 1);
    std::string outFileName = fName.substr(slashPos + 1, dotPos);

    cImage outImg = cImage(1, inImg.rows, inImg.columns);
    cImage intImg = cImage(1, inImg.rows, inImg.columns);
    cImage int_II_Img = cImage(1, inImg.rows, inImg.columns);
	
	tPoint t1 = timeNow();
	
	integral_image(inImg.chG, intImg.chG, inImg.rows, inImg.columns);
	integral_image_sqr(inImg.chG, int_II_Img.chG, inImg.rows, inImg.columns);

	int n = 7;
	int ncount = (2 * n + 1) * (2 * n + 1);
	
	float sum = 0;	//suma jasnosci
	float sum2 = 0;	//suma kwadratow jasnosci
	float m = 0.0;	//srednia jasnosc
	float s = 0.0;	//odchylenie standardowe
	float T = 0.0;
	float Rmax = 128.0;
	float k = 0.12f;
	
	for (unsigned int i = n + 1; i < inImg.rows - n; ++i) {
		for (unsigned int j = n +1; j < inImg.columns - n; ++j) {

			sum = intImg.chG[i + n][j + n] +
                  intImg.chG[i - n - 1][j - n - 1] -
                  intImg.chG[i - n - 1][j + n] -
                  intImg.chG[i + n][j - n - 1];
			sum2 = int_II_Img.chG[i + n][j + n] + 
                   int_II_Img.chG[i - n - 1][j - n - 1] -
                   int_II_Img.chG[i - n - 1][j + n] - 
                   int_II_Img.chG[i + n][j - n - 1];

			m = sum / ncount;
			s = sqrt(sum2 / ncount - m * m);
			
			T = m - 0.4 * s;    //Niblack method

			outImg.chG[i][j] = (inImg.chG[i][j] < T ) ?  0 : 255;
		}
	}

    tPoint t2 = timeNow();

    outImg.write("./output/" + outFileName + "_SauvolaBin" + "." + fileExt);    

    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();

    std::cout << outFileName + "." + fileExt + " Sauvola binarization time: " << duration1 << std::endl;
}