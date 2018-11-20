#include <string>
#include <chrono>
#include <iostream>
#include "../inc/cImage.h"

typedef std::chrono::high_resolution_clock::time_point tPoint;

tPoint timeNow() {return std::chrono::high_resolution_clock::now();}

int main (int argc, char *argv[]) { 

    //std::string inFileName = "./input/maly.pgm";
    std::string inFileName = "./input/016_lanczos.pgm";

    cImage inImg = cImage(inFileName);
    inImg.read();

    pixelArray outPixels;
    cImage::createPixelArray(outPixels, inImg.rows, inImg.columns);

    tPoint t1 = timeNow();
    

    for (unsigned int i = 0; i < inImg.rows; ++i) {
		for (unsigned int j = 0; j < inImg.columns; ++j){            
		    outPixels[i][j] = ( inImg.chG[i][j] > 128 ) ? 255 : 0;
		}
	}

    tPoint t2 = timeNow();

	for (unsigned int i = 0; i < inImg.rows * inImg.columns; ++i) {
		*(*outPixels + i) = (*(*inImg.chG + i) > 128 ) ?  255 : 0;
	}
    
    tPoint t3 = timeNow();

    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>( t3 - t2 ).count();

    std::cout << duration1;
    std::cout << duration2;

    cImage::deletePixelArray(outPixels);
    return 0;
}