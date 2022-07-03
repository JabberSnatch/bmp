#include <fstream>
#include <iostream>
#include <vector>

#define BMPTK_IMPLEMENTATION
#include "bmptk.h"

std::vector<uint8_t> LoadFile(char const* _path)
{
    std::ifstream source_file(_path, std::ios_base::binary);

    source_file.seekg(0, std::ios_base::end);
    size_t size = source_file.tellg();
    source_file.seekg(0, std::ios_base::beg);

    std::vector<uint8_t> memory{};
    memory.resize(size);

    source_file.read((char*)memory.data(), size);

    return memory;
}

int main(int argc, char const** argv)
{
    if (argc == 1)
    {
        std::cout << "Missing path to BMP file as first argument." << std::endl;
        return 1;
    }

    std::vector<uint8_t> memory = LoadFile(argv[1]);

    bmptk::BitmapFile bmpfile = {};
    bmptk::Result res = bmptk::LoadBMP(memory.data(), &bmpfile);
    if (res != bmptk::Result::Success)
    {
        std::cout << "Failed to load BMP." << std::endl;
        return 1;
    }

    std::cout << std::hex << (uintptr_t)bmpfile.memory << std::endl
              << (uintptr_t)bmpfile.pixel_data << std::endl
              << (ptrdiff_t)(bmpfile.pixel_data - bmpfile.memory) << std::endl;

    std::cout << std::dec << bmpfile.width << " " << bmpfile.height << std::endl;

    return 0;
}
