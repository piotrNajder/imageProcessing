#ifndef C_IMAGE_H
#define C_IMAGE_H

#include <string>

typedef unsigned char** pixelArray;

enum eFileType
{
    pgm = 0,
    ppm = 1,
    jpg = 3,
    jpeg = 4,
    unknown = 255
};

class cImage
{
private:
    std::string srcFileName = "";
    eFileType srcFileType = eFileType::unknown;
    uint8_t colorChannels = 0;

public:
    pixelArray chA = NULL;     // Alpha channel
    pixelArray chR = NULL;     // Red channel
    pixelArray chG = NULL;     // Greyscale or Green in case of RGB
    pixelArray chB = NULL;     // Blue channel

    int rows = 0;
    int columns = 0;
    int max_colors = 0;

public:

    cImage(const std::string fName);
    cImage(pixelArray gsArr, int r, int c);
    cImage(pixelArray rArr, pixelArray gArr, pixelArray bArr, int rows, int columns);
    cImage(int numOfColorChannels, int r, int c);
    ~cImage();

    bool read();
    bool write(const std::string fName);

    bool isGreyscale();
    bool isRgb();
    bool isRgba();

    static void createPixelArray(pixelArray arr, int rows, int columns);
    static void deletePixelArray(pixelArray arr);

private:
    int readPGMB_header();
    int readPGMB_data(int headerLength);
    int writePGMB_image(const std::string fname);

    int readPPMB_header();
    int readPPMB_data(int headerLength);
    int writePPMB_image(const std::string fname );

    void skipcomments(FILE *fp);
};

#endif