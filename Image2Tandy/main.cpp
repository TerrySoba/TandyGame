#include "lodepng/lodepng.h"

#include "tclap/CmdLine.h"

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
    return square(color1.r - color2.r) + square(color1.g - color2.g) + square(color1.b - color2.b);
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

struct Image
{
    std::vector<uint8_t> data; // RGBA, 8bit per channel
    unsigned int width, height;
};


Image loadPngImage(const std::string& filename)
{
    Image image;

    auto errorcode = lodepng::decode(image.data, image.width, image.height, filename, LCT_RGBA, 8);

    if (errorcode)
    {
        std::stringstream message;
        message << "Could not load input image. error:" << errorcode;
        throw std::runtime_error(message.str());
    }

    return image;
}


std::vector<uint8_t> convertToTandy(const Image& img)
{
    std::vector<uint8_t> tandyImage;
    tandyImage.reserve(img.data.size() / 8);

    uint32_t* data = (uint32_t*)img.data.data();

    for (unsigned int y = 0; y < img.height; ++y)
    {
        for (unsigned int x = 0; x < img.width / 2; ++x)
        {
            auto color1 = rgba2rgb(data[img.width*y + 2*x]);
            auto color2 = rgba2rgb(data[img.width*y + 2*x + 1]);

            unsigned char pixel1 = getBestTandyColor(color1);
            unsigned char pixel2 = getBestTandyColor(color2);

            unsigned char pixel = pixel1 << 4 | pixel2;
            tandyImage.push_back(pixel);
        }
    }

    return tandyImage;
}

void convertFile(std::string inputFile, std::string outputFile)
{
    auto image = loadPngImage(inputFile);

    std::cout << "w:" << image.width << " h:" << image.height << std::endl;

    auto tandyImage = convertToTandy(image);

    FILE* fp = fopen(outputFile.c_str(), "wb");

    int16_t width = image.width;
    int16_t height = image.height;

    fwrite(&width, 2, 1, fp);
    fwrite(&height, 2, 1, fp);
    fwrite(tandyImage.data(), tandyImage.size(), 1, fp);

    fclose(fp);
}


int main(int argc, char *argv[])
{
    try {

        TCLAP::CmdLine cmd(
            "This tool converts PNG images to images suitable for display on the Tandy1000 line of computers. "
            "The input image is converted to a 16 color image in the process.",
            ' ', "0.1");

        TCLAP::UnlabeledValueArg<std::string> inputPath("input", "The input PNG image to be converted.", true, "", "PNG image");
        cmd.add( inputPath );
        TCLAP::UnlabeledValueArg<std::string> outputPath("output", "The output image.", true, "", "output image");
        cmd.add( outputPath );
        
        cmd.parse(argc, argv);

        convertFile(inputPath.getValue(), outputPath.getValue());

        return 0;

    } catch (std::exception& ex) {
        std::cerr << "Caught exception: " << ex.what() << std::endl;
        return 2;
    }

}
