#include <fstream>
#include <iterator>
#include <algorithm>
#include "inc\cImage.h"

cImage::cImage(const std::string fName){
    std::ifstream infile(fName);
    if (infile.good()) {
        srcFileName = fName;
        std::size_t dotPos = fName.find_last_of(".");
        std::string fileExt = fName.substr(dotPos + 1);
        if (fileExt != "") {
            if (fileExt == "pgm") srcFileType == eFileType::pgm;
            else if (fileExt == "ppm") srcFileType == eFileType::ppm;
            // TODO: handle other 
            else {
                throw std::invalid_argument("File extension " + fileExt +" not supported");
            }
        }
    }
}

cImage::cImage(pixelArray gsArr, int r, int c) {
    rows = r; columns = c;
    chG = gsArr;
}
cImage::cImage(pixelArray rArr, pixelArray gArr, pixelArray bArr, int r, int c) {
    rows = r; columns = c;
    chR = rArr; chG = gArr; chB = bArr;
}

cImage::cImage(int numOfColorChannels, int r, int c) {

    if (numOfColorChannels == 1) {
        rows = r; columns = c;
        chG = new unsigned char* [rows];
        chG[0] = new unsigned char [rows * columns];
        for(int i = 1; i < rows; i++)
            chG[i] = chG[i-1] + columns;
    }
    else if (numOfColorChannels == 3) {
        rows = r; columns = c;
        chR = new unsigned char* [rows];
        chR[0] = new unsigned char [rows * columns];
        chG = new unsigned char* [rows];
        chG[0] = new unsigned char [rows * columns];
        chB = new unsigned char* [rows];
        chB[0] = new unsigned char [rows * columns];

        for(int i = 1; i < rows; i++) {
            chR[i] = chR[i-1] + columns;
            chG[i] = chG[i-1] + columns;
            chB[i] = chB[i-1] + columns;
        }
    }
    else {
        throw std::invalid_argument("Only picture with 1 or 3 colors channels can be created.");
    }
}

cImage::~cImage(){
    if (chA[0] != NULL) delete[] chA[0];
    if (chA != NULL) delete[] chA;
    if (chR[0] != NULL) delete[] chR[0];
    if (chR != NULL) delete[] chR;
    if (chG[0] != NULL) delete[] chG[0];
    if (chG != NULL) delete[] chG;
    if (chB[0] != NULL) delete[] chB[0];
    if (chB != NULL) delete[] chB;
}

bool cImage::read() {
    if(srcFileName != "" && srcFileType != eFileType::unknown) {
        switch( srcFileType ) {
            case eFileType::pgm:
                int hpos;
                if ( (hpos = readPGMB_header() <= 0 )) return false;
                chG = new unsigned char* [rows];
                chG[0] = new unsigned char [rows * columns];
                for(int i = 1; i < rows; i++)
                    chG[i] = chG[i-1] + columns;
	            if( readPGMB_data(hpos) == 0 ) return false;
                break;
            case eFileType::ppm:
                int hpos;
                if ( (hpos = readPGMB_header() <= 0 )) return false;

                chR = new unsigned char* [rows];
                chR[0] = new unsigned char [rows * columns];
                chG = new unsigned char* [rows];
                chG[0] = new unsigned char [rows * columns];
                chB = new unsigned char* [rows];
                chB[0] = new unsigned char [rows * columns];

                for(int i = 1; i < rows; i++) {
                    chR[i] = chR[i-1] + columns;
                    chG[i] = chG[i-1] + columns;
                    chB[i] = chB[i-1] + columns;
                }

	            if( readPGMB_data(hpos) == 0 ) return false;

                break; 
            default:
                throw std::invalid_argument("File extension not supported");
                break;
        }
        return true;
    }
    else return false;      // no name specified
}

bool cImage::write(const std::string fName) {

}

bool cImage::isGreyscale() {
    if (chA == NULL && chR == NULL &&
        chB == NULL && chG != NULL) {
            return true;
        }
    else return false;
}

bool cImage::isRgb() {
    if (chA == NULL && chR != NULL &&
        chB != NULL && chG != NULL) {
            return true;
        }
    else return false;
}

bool cImage::isRgba() {
    if (chA != NULL && chR != NULL &&
        chB != NULL && chG != NULL) {
            return true;
        }
    else return false;
}

void cImage::skipcomments(FILE *fp) {
	int ch;
	char line[256];

	while ((ch = fgetc(fp)) != EOF && isspace(ch))
		;
	if (ch == '#') {
		fgets(line, sizeof(line), fp);
		skipcomments(fp);
	} else
		fseek(fp, -1, SEEK_CUR);
}

int cImage::readPGMB_header() {
    FILE *fp;
	size_t flen, hlen;
	char signature[3];

	if((fp = fopen(srcFileName.c_str(), "rb")) == NULL) {
        return 0;
    }		

	fseek(fp, 0, SEEK_END);
    flen = ftell(fp);	//file lenght
	fseek(fp, 0, SEEK_SET);

	fgets(signature, sizeof(signature), fp);
	if (signature[0] != 'P' || signature[1] != '5') {
        //probably not pgm binary file...
        fclose(fp);
        return 0;
    }

	skipcomments(fp); 
	fscanf(fp, "%d", columns);
	skipcomments(fp);
	fscanf(fp, "%d", rows);
	skipcomments(fp);
	fscanf(fp, "%d", max_colors);
	fgetc(fp);

	hlen = ftell(fp); //header lenght
	fclose(fp); 
	if ( rows * columns != (flen - hlen) )	//we assume only one picture in the file
		return 0;

	return hlen;	
}

int cImage::readPGMB_data(int headerLength) {
    FILE *fp;
	if((fp = fopen(srcFileName.c_str(), "rb")) == NULL) return 0;
	fseek(fp, headerLength, SEEK_SET);
	int readedrows = fread(chG, columns, rows, fp);
	fclose(fp);
	
	if(rows != readedrows)
		return 0;
	return 1;
}

int cImage::writePGMB_image(const std::string fname) {
    FILE *fp;

	if((fp = fopen(fname.c_str(), "wb")) == NULL) return(0);

	fprintf(fp, "P5\n%d %d\n# eyetom.com\n%d\n", columns, rows, max_colors);

	if(rows != fwrite(chG[0], columns, rows, fp)){
		fclose(fp);
		return(0);
	}

	fclose(fp);
	return(1);
}

int cImage::readPPMB_header() {

    FILE *fp;
	size_t flen, hlen;
	char signature[3];

	if((fp=fopen(srcFileName.c_str(), "rb")) == NULL) return 0;

	fseek(fp, 0, SEEK_END);
    flen = ftell(fp);	//file lenght
	fseek(fp, 0, SEEK_SET);

	fgets(signature, sizeof(signature), fp);
	if (signature[0] != 'P' || signature[1] != '6') 
		{ fclose(fp); return 0; }	//probably not pgm binary file...

	skipcomments(fp); 
	fscanf(fp, "%d", columns);
	skipcomments(fp);
	fscanf(fp, "%d", rows);
	skipcomments(fp);
	fscanf(fp, "%d", max_colors);
	fgetc(fp);

	hlen = ftell(fp); //header lenght
	fclose(fp); 
	if ( rows * 3 * columns  != (flen - hlen) )	//we assume only one picture in the file
		return 0;

	return hlen;
}

int cImage::readPPMB_data(int headerLength) {
    long i, wxh;
	FILE *fp;
	
	if (max_colors > 255) return 0;	//for now only 1 byte color values

	if((fp = fopen(srcFileName.c_str(), "rb")) == NULL) return 0;

	fseek(fp, headerLength, SEEK_SET);

	wxh = rows * columns;
	for(i = 0; i < wxh; i++) {
		*(*chR + i) = (unsigned char)fgetc(fp);
		*(*chG + i) = (unsigned char)fgetc(fp);
		*(*chB + i) = (unsigned char)fgetc(fp);
	}
	return 1;
}

int cImage::writePPMB_image(const std::string fname ) {
    long i, wxh;
	FILE *fp;

	if((fp = fopen(srcFileName.c_str(), "wb")) == NULL) return(0);

	fprintf(fp, "P6\n%d %d\n# eyetom.com\n%d\n", columns, rows, max_colors);

	wxh = rows * columns;
	for(i = 0; i < wxh; i++) {
        /* Write the image in pixel interleaved format. */
		fputc(*(*chR + i), fp);
		fputc(*(*chG + i), fp);
		fputc(*(*chB + i), fp);
	}

	fclose(fp);
	return(1);
}

void cImage::createPixelArray(pixelArray arr, int rows, int columns) {
    arr = new unsigned char* [rows];
    arr[0] = new unsigned char [rows * columns];
    for(int i = 1; i < rows; i++)
        arr[i] = arr[i-1] + columns;        
}

void cImage::deletePixelArray(pixelArray arr) {
    if (arr[0] != NULL) delete[] arr[0];
    if (arr != NULL) delete[] arr;
}