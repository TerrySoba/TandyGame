#include "lodepng/lodepng.h"

#include <iostream>
#include <exception>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <sstream>


double square(double val)
{
    return val * val;
}

struct RGB
{
    unsigned char r, g, b;
};

double colorDiff(
        const RGB& color1,
        const RGB& color2)
{
    // very simplistic euclidean distance...
    return std::sqrt(square(color1.r - color2.r) + square(color1.g - color2.g) + square(color1.b - color2.b));
}

static const std::vector<std::pair<char, RGB>> tandyColors =
{
    { 0 , {0x00,0x00,0x00}},
    { 1 , {0x00,0x00,0xAA}},
    { 2 , {0x00,0xAA,0x00}},
    { 3 , {0x00,0xAA,0xAA}},
    { 4 , {0xAA,0x00,0x00}},
    { 5 , {0xAA,0x00,0xAA}},
    { 6 , {0xAA,0x55,0x00}},
    { 7 , {0xAA,0xAA,0xAA}},
    { 8 , {0x55,0x55,0x55}},
    { 9 , {0x55,0x55,0xFF}},
    {10 , {0x55,0xFF,0x55}},
    {11 , {0x55,0xFF,0xFF}},
    {12 , {0xFF,0x55,0xFF}},
    {13 , {0xFF,0x55,0xFF}},
    {14 , {0xFF,0xFF,0x55}},
    {15 , {0xFF,0xFF,0xFF}},
};



unsigned char getBestTandyColor(RGB color)
{
    char bestIndex = 0;
    double bestDiff = 100000;
    for (const auto& entry : tandyColors)
    {
        double diff;
        if ((diff = colorDiff(color, entry.second)) < bestDiff)
        {
            bestIndex = entry.first;
            bestDiff = diff;
        }
    }

    return bestIndex;
}

RGB rgba2rgb(uint32_t rgba)
{
    return RGB{
        (uint8_t)((rgba & 0x000000ff) >> 0),
        (uint8_t)((rgba & 0x0000ff00) >> 8),
        (uint8_t)((rgba & 0x00ff0000) >> 16),
        };
}

void convertFile(std::string inputFile, std::string outputFile)
{
    std::vector<unsigned char> imageData;
    unsigned w, h;

    auto errorcode = lodepng::decode(imageData, w, h, inputFile, LCT_RGBA, 8);

    if (errorcode)
    {
        std::stringstream message;
        message << "Could not load input image. error:" << errorcode;
        throw std::runtime_error(message.str());
    }

    std::cout << "w:" << w << " h:" << h << std::endl;

    FILE* fp = fopen(outputFile.c_str(), "wb");

    int16_t width = w;
    int16_t height = h;

    fwrite(&width, 2, 1, fp);
    fwrite(&height, 2, 1, fp);

    uint32_t* data = (uint32_t*)imageData.data();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width / 2; ++x)
        {
            auto color1 = rgba2rgb(data[w*y + 2*x]);
            auto color2 = rgba2rgb(data[w*y + 2*x + 1]);

            unsigned char pixel1 = getBestTandyColor(color1);
            unsigned char pixel2 = getBestTandyColor(color2);

            unsigned char pixel = pixel1 << 4 | pixel2;
            fwrite(&pixel, 1, 1, fp);
        }
    }

    fclose(fp);
}


int main(int argc, char *argv[])
{
    try {
        if (argc != 3)
        {
            std::cerr << "Usage: " << argv[0] << " input_file output_file" << std::endl;
            return 1;
        }

        convertFile(argv[1], argv[2]);

        return 0;

    } catch (std::exception& ex) {
        std::cerr << "Caught exception: " << ex.what() << std::endl;
        return 2;
    }

}
