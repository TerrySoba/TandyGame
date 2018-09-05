#include <QCoreApplication>

#include <QImage>

#include <iostream>
#include <exception>

#include <stdio.h>
#include <cmath>


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

unsigned char getBestTandyColor(QRgb colorValue)
{
    RGB color;
    color.r = qRed(colorValue);
    color.g = qGreen(colorValue);
    color.b = qBlue(colorValue);
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

void convertFile(QString inputFile, QString outputFile)
{
    auto image = QImage(inputFile);

    if (image.isNull())
    {
        throw std::runtime_error("Could not load input file.");
    }

    FILE* fp = fopen(outputFile.toStdString().c_str(), "wb");

    int16_t width = image.width();
    int16_t height = image.height();

    fwrite(&width, 2, 1, fp);
    fwrite(&height, 2, 1, fp);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width / 2; ++x)
        {
            auto color1 = image.pixel(2 * x    ,y);
            auto color2 = image.pixel(2 * x + 1,y);

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
        QCoreApplication a(argc, argv);

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



