#include <iostream>
#include <cstdio>
#include <vector>
#include <exception>
#include <sstream>
#include <map>

#include "cJSON.h"


std::string UTF8toISO8859_1(const char* in)
{
    std::string out;
    if (in == NULL)
        return out;

    unsigned int codepoint;
    while (*in != 0)
    {
        unsigned char ch = static_cast<unsigned char>(*in);
        if (ch <= 0x7f)
            codepoint = ch;
        else if (ch <= 0xbf)
            codepoint = (codepoint << 6) | (ch & 0x3f);
        else if (ch <= 0xdf)
            codepoint = ch & 0x1f;
        else if (ch <= 0xef)
            codepoint = ch & 0x0f;
        else
            codepoint = ch & 0x07;
        ++in;
        if (((*in & 0xc0) != 0x80) && (codepoint <= 0x10ffff))
        {
            if (codepoint <= 255)
            {
                out.append(1, static_cast<char>(codepoint));
            }
            else
            {
                // do whatever you want for out-of-bounds characters
            }
        }
    }
    return out;
}


std::vector<char> loadFile(const std::string& filename)
{
    FILE *f = fopen(filename.c_str(), "rb");

    if (!f)
    {
        std::stringstream message;
        message << "Could not open file '" << filename << "' for reading.";
        throw std::runtime_error(message.str());
    }

    // Determine file size
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);

    std::vector<char> buffer(size+1);
    buffer[size] = '\0';

    rewind(f);
    fread(buffer.data(), sizeof(char), size, f);

    return buffer;
}

std::map<int, std::string> parseTranslationFile(const std::string &path)
{
    auto jsonData = loadFile(path);

    auto json = cJSON_Parse(jsonData.data());
    if (json == nullptr)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        std::stringstream message;
        message << "Error in JSON before: " << error_ptr;
        throw std::runtime_error(message.str());
    }

    if (!cJSON_IsObject(json))
    {
        throw std::runtime_error("Root element of JSON is not an object.");
    }

    std::map<int, std::string> ret;

    cJSON *translation;
    cJSON_ArrayForEach(translation, json)
    {
        if (!cJSON_IsString(translation))
        {
            throw std::runtime_error("Translation must be a string.");
        }

        try
        {
            ret[std::stoi(translation->string)] = translation->valuestring;
        }
        catch(...)
        {
            throw std::runtime_error("Could not convert integer.");
        }        
    }

    return ret;
}


void writeBinaryTranslationFile(const std::map<int, std::string>& translations, const std::string& filename)
{
    // file format
    //   "TEXT" (4 bytes identifier)
    //   entry count (2bytes)
    //   --- repeated for each entry
    //     entry id (2bytes)
    //     entry offset (4bytes)
    //     entry length (2bytes)
    //   --- repeated for each entry
    //     entry text (length given in TOC)

    FILE* fp = fopen(filename.c_str(), "wb");
    if (!fp)
    {
        std::stringstream message;
        message << "Could not open file '" << filename << "' for writing.";
        throw std::runtime_error(message.str());
    }

    fwrite("TEXT", 4, 1, fp);
    uint16_t entryCount = translations.size();
    fwrite(&entryCount, 2, 1, fp);

    const uint32_t HEADER_SIZE = 6;
    const uint32_t ENTRY_SIZE = 8;

    uint32_t textLenSum = 0;

    for (auto translation : translations)
    {
        uint16_t id = translation.first;
        fwrite(&id, 2, 1, fp);
        uint32_t offset = HEADER_SIZE + ENTRY_SIZE * translations.size() + textLenSum;
        fwrite(&offset, 4, 1, fp);
        auto str = UTF8toISO8859_1(translation.second.c_str());
        uint16_t len = str.size();
        fwrite(&len, 2, 1, fp);
        textLenSum += len + 1;
    }

    for (auto translation : translations)
    {
        auto str = UTF8toISO8859_1(translation.second.c_str());
        fwrite(str.c_str(), str.size() + 1, 1, fp);
    }

    fclose(fp);
}


void printUsage(const char* exeName)
{
    std::cout << "Usage: " << exeName << " <input file json> <output file binary>\n";
}


int main(int argc, char* argv[])
{
    try
    {

        if (argc != 3)
        {
            printUsage(argv[0]);
            return 1;
        }

        auto translations = parseTranslationFile(argv[1]);
        writeBinaryTranslationFile(translations, argv[2]);

        std::cout << "Converted " << translations.size() << " text entries.\n";

        // for (auto r : translations)
        // {
        //     std::cout << r.first << ":" << r.second << "\n";
        // }


    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception: \"" << e.what() << "\"\n";
        return 2;
    }
    catch(...)
    {
        std::cerr << "Unknown Exception\n";
        return 3;
    }


    return 0;
}
