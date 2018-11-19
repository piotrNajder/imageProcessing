#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <ctime>
#include "TPGM.h"

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

int main(int argc, char **argv)
{
	int rows, cols;           
	int max_color;
	int hpos, i,j;		

	std::string infname = "maly.ppm";

	if ( (hpos = readPPMB_header(infname.c_str(), &rows, &cols, &max_color)) <= 0 )	   exit(1);

	unsigned char **R = new unsigned char* [rows];
	R[0] = new unsigned char [rows*cols];
	for(int i = 1; i < rows; i++)
		R[i] = R[i-1] + cols;

	unsigned char **G = new unsigned char* [rows];
	G[0] = new unsigned char [rows*cols];
	for(int i = 1; i < rows; i++)
		G[i] = G[i-1] + cols;

	unsigned char **B = new unsigned char* [rows];
	B[0] = new unsigned char [rows*cols];
	for(int i = 1; i < rows; i++)
		B[i] = B[i-1] + cols;

	if( readPPMB_data(R[0], G[0], B[0], infname.c_str(), hpos, rows, cols, max_color) == 0 )	   exit(1);

	//zamiana obrazu kolorowego na skalê szarosci

	//przzygotowanie obrazu grayscale
	unsigned char **a = new unsigned char* [rows];
	a[0] = new unsigned char [rows*cols];
	for(int i = 1; i < rows; i++)
		a[i] = a[i-1] + cols;

	unsigned char _r,_g,_b, gray_value;

	for (i = 0; i < rows ; ++i){
		for (j = 0; j < cols ; ++j){
			_r = R[i][j]; 
			_g = G[i][j]; 
			_b = B[i][j];
			gray_value = (unsigned char) ((0.299 * _r) + (0.587 * _g) + (0.114 * _b));
			a[i][j] = gray_value;
		}
	}

	//przygotowanie czarno-bialej tablicy wyjsciowej
	unsigned char **b = new unsigned char* [rows];
	b[0] = new unsigned char [rows*cols];
	for(int i = 1; i < rows; i++)
		b[i] = b[i-1] + cols;
	
	clock_t begin = clock();

	//for (i = 0; i < rows; ++i){
		//for (j = 0; j < cols; ++j){
			//b[i][j] = (a[i][j] > 128 ) ?  255 : 0;
		//}
	//}
	for (i = 0; i < rows * cols; ++i) {
		*(*b + i) = (*(*a + i) > 128 ) ?  255 : 0;
	}
	

	clock_t end = clock();
  	double elapsed = double(end - begin) / CLOCKS_PER_SEC;

	std::string outfname = infname;
	replace(outfname, ".ppm", "_col2gray_simple.pgm");

	if( writePGMB_image(outfname.c_str(), b[0], rows, cols, 255) == 0)	   exit(1);

	delete[] R[0]; delete[] R;
	delete[] G[0]; delete[] G;
	delete[] B[0]; delete[] B;
	delete[] a[0]; delete[] a;
	delete[] b[0]; delete[] b;

   printf("czas binaryzacji : %f ms\n", elapsed);

   return 0;
}
