#ifndef C_IMAGE_H
#define C_IMAGE_H

#include <string>

enum eFileType
{
    pgm = 0,
    ppm = 1,
    jpg = 3,
    jpeg = 4,
    unknown = 255
};

template <typename T = unsigned char>
class cImage
{
private:
    std::string srcFileName = "";
    eFileType srcFileType = eFileType::unknown;
    uint8_t colorChannels = 0;

public:
	T **chA = nullptr;     // Alpha channel
	T **chR = nullptr;     // Red channel
	T **chG = nullptr;     // Greyscale or Green in case of RGB
	T **chB = nullptr;     // Blue channel

    unsigned int rows = 0;
    unsigned int columns = 0;
    unsigned int max_colors = 0;

public:

    cImage(const std::string fName);
    cImage(T **gsArr, unsigned int r, unsigned int c);
    cImage(T **rArr, T **gArr, T **bArr, unsigned int rows, unsigned int columns);
	cImage(uint8_t numOfColorChannels, unsigned int r, unsigned int c, unsigned int max_col = 255);
    ~cImage();

    bool read();
    bool write(const std::string fName);

    bool isGreyscale();
    bool isRgb();
    bool isRgba();

private:
    int readPGMB_header();
    int readPGMB_data(unsigned int headerLength);
    int writePGMB_image(const std::string fname);

    int readPPMB_header();
    int readPPMB_data(unsigned int headerLength);
    int writePPMB_image(const std::string fname );

    void skipcomments(FILE *fp);
};

#endif